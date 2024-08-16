import fs from 'fs';
import path from 'path';

const fsPath = (filePath: string, filename: string) => {
  return new Promise((resolve, reject) => {
    const fileAttr = filename.split('$');
    fs.stat(filePath, (err, stats) => {
      if (err) {
        console.log(err);
        reject(err);
      } else {
        const result = {
          name: fileAttr[4],
          platform: fileAttr[3],
          uploadedBy: fileAttr[2],
          size: Number(stats.size),
          modifiedAt: Number(stats.mtime),
          uploadedAt: Number(stats.mtime),
        };
        resolve(result);
      }
    });
  });
};

export const yarafile_list = async (req, res) => {
  const directoryPath = path.join(process.cwd(), 'uploads/yara');
  const fileList = [];

  try {
    const filenames = fs.readdirSync(directoryPath);
    // console.log('\nFilenames in directory:');

    for (const filename of filenames) {
      const filePath = path.join(directoryPath, filename);
      const result = await fsPath(filePath, filename);
      fileList.push(result);
    }

    // console.log('File List: ', fileList);
    fileList.sort((a, b) => b.uploadedAt - a.uploadedAt);

    return res.customSuccess(200, 'Yara files', fileList);
  } catch (error) {
    console.error('Error in adding Yara file', error);
    return res.status(500).json({ status: 'Fail', message: 'Error in listing Yara files. Internal Server Error' });
  }
};
