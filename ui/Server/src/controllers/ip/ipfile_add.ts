import { dbSourceConnection } from 'index';
import fs from 'fs';
import { parse } from 'fast-csv';

function isValidIPAddress(ipAddress: string): boolean {
  if(!ipAddress|| ipAddress === ''||ipAddress ===undefined) return false;
  // Regular expression pattern to match a valid IPv4 address
  const ipv4Pattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;

  // Check if it matches the IPv4 pattern
  const ipv4Match = ipAddress.match(ipv4Pattern);

  if (ipv4Match) {
    // Check each octet of the IP address
    for (let i = 1; i <= 4; i++) {
      const octet = parseInt(ipv4Match[i], 10);
      if (octet < 0 || octet > 255) {
        return false; // Octet out of valid range
      }
    }
    return true; // All octets are valid
  }

  // Regular expression pattern to match a valid IPv6 address
  const ipv6Pattern = /^([\da-fA-F]{1,4}:){7}[\da-fA-F]{1,4}$/;

  // Check if it's a valid IPv6 address
  return ipv6Pattern.test(ipAddress);
}

export const ipfile_add = async (req: any, res: any) => {
  let data = [];
  let newIps = [];
  
  try {
    const uploadedFile = req.file;
    const fileDetails = {
      filename: uploadedFile.filename,
      originalname: uploadedFile.originalname,
      mimetype: uploadedFile.mimetype,
      size: uploadedFile.size,
      destination: uploadedFile.destination,
    };

    const path = process.cwd() + "/" + uploadedFile.destination;
    fs.createReadStream(path + "/" + uploadedFile.filename)
      .pipe(parse({ headers: true }))
      .on('error', error => console.error(error))
      .on('data', row => data.push(row))
      .on('end', async () => {
        console.log("Data: ", data);

        // IP INSERTION LOGIC
        const ips = data.map((item: any) => item.ip);
        const entityManager = dbSourceConnection()[1];

        // Fetch existing IPs from the database
        const existingIps = await entityManager.query(`SELECT ip from ip_tables;`);

        // Filter new IPs that are valid and not already in the database
        newIps = ips.filter((item: string) => isValidIPAddress(item) && !existingIps.some((existingIp: any) => existingIp.ip === item));

        // Insert new IPs into the ip_tables table (you'll need to adapt this part based on your database schema)
        for (const ip of newIps) {
          if(ip !== "")
          {
            await entityManager.query(`INSERT INTO ip_tables (ip) VALUES ('${ip}')`);
          }
          
        }

        res.status(200).json({ status: "Success", message: 'File uploaded successfully', file: fileDetails, newIps });
      });

  } catch (error) {
    console.error("Error in adding Ip file", error);
    res.status(500).json({ status: "Fail", message: "Error in adding Ip file. Internal Server Error" });
  }
};
