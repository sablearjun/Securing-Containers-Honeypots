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

const cve_sorter = (t:any) =>{
    const sortedCVEs = t.sort((a, b) => {
    // Extract years and numbers from the CVE strings
    const [, yearA, numberA] = a.match(/(\d+)-(\d+)/) || [];
    const [, yearB, numberB] = b.match(/(\d+)-(\d+)/) || [];
    
    // Convert years and numbers to numbers for comparison
    const yearComparison = parseInt(yearA, 10) - parseInt(yearB, 10);
    const numberComparison = parseInt(numberA, 10) - parseInt(numberB, 10);
    
    // If years are different, use the year comparison
    // If years are the same, use the number comparison
    return yearComparison !== 0 ? yearComparison : numberComparison;
    });
    return sortedCVEs;
}

export const node_programs_list = async(req: Request, res: Response) => {
    let host_identifier: string = '';
    let os_name : string = '';

    const time = new Date().getTime();
    const lastTime = Number(Math.floor(time/1000)) - 83600;

    if(req.query.hasOwnProperty('host_identifier')){
        host_identifier= String(req.query['host_identifier']);
    }

    if(req.query.hasOwnProperty('os_name')){
        os_name= String(req.query['os_name']);
    }

    if(host_identifier=='' || os_name==''){
        return res.status(400).json({ message: "Bad Request"});
    }

    try{
        const entityManager = dbSourceConnection()[1];

        let result;
        const programsQuery = `SELECT * FROM programs where host_identifier = $1 order by unixtime desc`;
        const debPackagesQuery = `SELECT * from deb_packages where host_identifier = $1 order by unixtime desc`;
        const chocoPackagesQuery = `SELECT * from chocolatey_packages where host_identifier = $1 order by unixtime desc`;
        const rpmPackagesQuery = `SELECT * from chocolatey_packages where host_identifier = $1 order by unixtime desc`;
        const chromeExtensionsQuery = `SELECT * from chrome_extensions where host_identifier = $1 order by unixtime desc`;
        const npmPackagesQuery = `SELECT * from npm_packages where host_identifier = $1 order by unixtime desc`;
        const portagePackagesQuery = `SELECT * from portage_packages where host_identifier = $1 order by unixtime desc`;
        const ieExtensionQuery = `SELECT * from ie_extensions where host_identifier = $1 order by unixtime desc`;
        const firefoxAddOnsQuery = `SELECT * from firefox_addons where host_identifier = $1 order by unixtime desc`;

        // const pythonPackagesQuery = `SELECT * from python_packages where host_identifier = $1 order by unixtime desc`;
        // const pythonPackages = await entityManager.query(pythonPackagesQuery, [host_identifier]);

        if(os_name.includes('indows')){

            // Result for Windows only nodes:

            const programs = await entityManager.query(programsQuery, [host_identifier]);
            const chocoPackages = await entityManager.query(chocoPackagesQuery, [host_identifier]);
            const chromeExtensions = await entityManager.query(chromeExtensionsQuery, [host_identifier]);
            const npmPackages = await entityManager.query(npmPackagesQuery, [host_identifier]);
            const ieExtensions = await entityManager.query(ieExtensionQuery, [host_identifier]);
            const firefoxAddOns = await entityManager.query(firefoxAddOnsQuery, [host_identifier]);

            // Taging the programs 
            let tag = '';
            const vulProgramsPieChart = { "LOW" : 0 , "MEDIUM" : 0, "HIGH" : 0};
            for (let i = 0; i < programs.length; i++) {
                if(programs[i].cve_match){
                    let programInfo = stringToArray(programs[i], 'cve_match');
                    programInfo['severity'] = vulnerabilityTagger(programInfo);
                    let cve_ids = cve_sorter(programInfo['cve_match'].map((it:any) => it.cve_id));
                    if(cve_ids.length > 2){
                        programInfo['first_reported_at'] = cve_ids.slice(0,2).join(',');
                    }else{
                        programInfo['first_reported_at'] = cve_ids.join(',');
                    }
                    tag = programInfo['severity'];
                    // Update pie chart data
                    vulProgramsPieChart[tag] += 1;
                    // Update the original programs array with processed information
                    programs[i] = programInfo;
                }
            }

            let programsPieChart = [];
            if(Object.keys(vulProgramsPieChart).reduce((total:number, key:string) => total + vulProgramsPieChart[key], 0) ==0){
                programsPieChart = undefined;
            }else{
                programsPieChart =  [
                    {
                        "name" : "LOW",
                        "count" : vulProgramsPieChart['LOW']
                    },
                    {
                        "name" : "MEDIUM",
                        "count" : vulProgramsPieChart['MEDIUM']
                    },
                    {
                        "name" : "HIGH",
                        "count" : vulProgramsPieChart['HIGH']
                    },
                ]
            }

            result = {
                "programs" : programs,
                "choco" : chocoPackages,
                "chrome_extensions" : chromeExtensions,
                "npm_packages" : npmPackages,
                "ie_extensions" : ieExtensions,
                "firefox_addons" : firefoxAddOns,
                "vul_programs_piechart" : programsPieChart
            }
        }else{
            const debPackages = await entityManager.query(debPackagesQuery, [host_identifier]);
            const rpmPackages = await entityManager.query(rpmPackagesQuery, [host_identifier]);
            const chromeExtensions = await entityManager.query(chromeExtensionsQuery, [host_identifier]);
            const npmPackages = await entityManager.query(npmPackagesQuery, [host_identifier]);
            const portagePackages = await entityManager.query(portagePackagesQuery, [host_identifier]);
            const firefoxAddOns = await entityManager.query(firefoxAddOnsQuery, [host_identifier]);

            result = {
                "deb" : debPackages,		// Linux
                "rpm" :  rpmPackages,		// Linux
                "chrome_extensions": chromeExtensions,	// Windows // Linux
                "npm_packages": npmPackages,		// Windows // Linux
                "portage_packages": portagePackages,	// Linux
                "firefox_addons": firefoxAddOns,		// Windows // Linux
            }

        }
        return res.status(200).json(result);
    }catch(err){
        console.error("Error in fetching programs list: ", err);
        return res.status(500).json({ message: "Error in fetching programs list"});
    }
}