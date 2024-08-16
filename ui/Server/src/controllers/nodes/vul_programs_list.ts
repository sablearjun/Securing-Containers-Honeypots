import { dbSourceConnection } from 'index';
import { Request, Response, NextFunction } from 'express';

const stringToArray = (rowData:any, column : string)=>{
    if(rowData !== undefined && typeof(rowData[column]) === "string"){
      const tmp = rowData[column];
      rowData[column] = JSON.parse(tmp);
    }
    return rowData;
}

const vulnerabilityTagger = (program: any) =>{
    let id = 0;
    const labels = ["LOW", "MEDIUM", "HIGH"];
    let lvl;
    for(const cve of program?.cve_match){
        lvl = cve.baseSeverity;
        // console.log("lvl", lvl, labels[id]);
        if(lvl === "MEDIUM" && id<1){
            id = 1;
        }else if(lvl === "HIGH" && id<=2){
            id = 2;
            return labels[id];
        }
    }
    return labels[id];
}

export const vul_programs_list = async(req: Request, res: Response) => {

    try{
        const entityManager = dbSourceConnection()[1];

        let result, node_names;
        let programs = [];

        const query = `SELECT name, version, publisher, cve_match, COUNT(DISTINCT host_identifier) AS nodes_count FROM programs where is_cve_processed=true GROUP BY name, version, publisher, cve_match`;
        programs = await entityManager.query(query);

        const pieChartData = { "LOW" : 0 , "MEDIUM" : 0, "HIGH" : 0};
        const nodeVulnerabilityCount = { "LOW" : {}, "HIGH" : {}, "MEDIUM" : {}};
        //Eg. nodeVulnerabilityCount = {"LOw" :{ "node 1" : 1, "node 2" : 45}, "HIGH" :{ "node 2" : 6, "node 2" : 6},"MEDIUM" :{ "node 1" : 5, "node 2" : 6}}

        let tag = '';
        for (let i = 0; i < programs.length; i++) {
            let programInfo = stringToArray(programs[i], 'cve_match');
            programInfo['severity'] = vulnerabilityTagger(programInfo);
            tag = programInfo['severity'];
            programInfo['nodes_count'] = parseInt(programInfo['nodes_count']);

            // Fetch node information for the current program
            node_names = await entityManager.query(
                `SELECT n.hostname, n.host_identifier FROM nodes n JOIN programs p ON n.host_identifier = p.host_identifier WHERE p.name = $1 AND p.version = $2`,
                [programInfo.name, programInfo.version]
            );
            programInfo['nodes'] = node_names;

            //update nodecount
            node_names.forEach((it:any) => {
                if(it.hostname in nodeVulnerabilityCount[tag]){
                    nodeVulnerabilityCount[tag][it.hostname] +=1;
                }else{
                    nodeVulnerabilityCount[tag][it.hostname] = 1;
                }
            });
            //

            // Update pie chart data
            pieChartData[tag] += 1;

            // Update the original programs array with processed information
            programs[i] = programInfo;
        }

        for (const key in nodeVulnerabilityCount) {
            if (nodeVulnerabilityCount.hasOwnProperty(key)) {
              // Sort the inner object based on values in descending order
              nodeVulnerabilityCount[key] = Object.fromEntries(
                Object.entries(nodeVulnerabilityCount[key]).sort((a:any, b:any) => b[1] - a[1])
              );
            }
        }

        let programsPieChart = [];
        if(Object.keys(pieChartData).reduce((total:number, key:string) => total + pieChartData[key], 0) ==0){
            programsPieChart = undefined;
        }else{
            programsPieChart = [
                {
                    "name" : "LOW",
                    "count" : pieChartData['LOW']
                },
                {
                    "name" : "MEDIUM",
                    "count" : pieChartData['MEDIUM']
                },
                {
                    "name" : "HIGH",
                    "count" : pieChartData['HIGH']
                },
            ]
        }

        programs.sort((a:any, b:any) => {
            const order = { HIGH: 1, MEDIUM: 2, LOW: 3 };
            return order[a.severity] - order[b.severity];
        });

        result = {
            "programs" : programs,
            "piechart" : programsPieChart,
            "nodeCount" : nodeVulnerabilityCount,
            "nodeSummary" : { "HIGH": Object.keys(nodeVulnerabilityCount.HIGH).length,
            "MEDIUM" : Object.keys(nodeVulnerabilityCount.MEDIUM).length,
            "LOW": Object.keys(nodeVulnerabilityCount.LOW).length}
        }

        return res.status(200).json(result);
    }catch(err){
        console.error("Error in fetching programs list: ", err);
        return res.status(500).json({ message: "Error in fetching programs list"});
    }
}