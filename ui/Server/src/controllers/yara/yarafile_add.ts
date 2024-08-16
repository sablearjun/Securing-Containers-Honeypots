import { dbSourceConnection } from 'index';
import fs from 'fs';

export const yarafile_add = async (req: any, res: any) => {

  try {
    const uploadedFile = req.file;
    const fileDetails = {
      filename: uploadedFile.filename,
      originalname: uploadedFile.originalname,
      mimetype: uploadedFile.mimetype,
      size: uploadedFile.size,
      destination: uploadedFile.destination,
    };

    res.status(200).json({ status: "Success", message: 'File uploaded successfully', file: fileDetails,});
  //   To DO
  } catch (error) {
    console.error("Error in adding Yara ile", error);
    res.status(500).json({ status: "Fail", message: "Error in adding Yara file. Internal Server Error" });
  }
};
