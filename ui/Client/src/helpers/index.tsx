import { LOGOUT_DELETE_TOKEN_URL } from "../apis/urls";

export function timeConverter(UNIX_timestamp: number) {
    //console.log(UNIX_timestamp)
      var a = new Date(UNIX_timestamp * 1000);
      var months = [
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec",
      ];
      var year = a.getFullYear();
      var month = months[a.getMonth()];
      var date = ()=>{
        if(a.getDate()<10){
          return "0"+a.getDate()
        }
        return a.getDate()
      };
      var hour = a.toLocaleString('en-US', {
        hour: 'numeric',
        minute: 'numeric',
        hour12: true
      });
      // var min = a.getMinutes();
      // var sec = a.getSeconds();
      var time =
        date() + " " + month + " " + year + " " + hour;
      return time+" (IST)";
  }

  export const fileName=(name:string,subName?:string)=>{
    var today = new Date();
    var dd = String(today.getDate()).padStart(2, '0');
    var mm = String(today.getMonth() + 1).padStart(2, '0'); //January is 0!
    var yyyy = today.getFullYear();
    var hr=today.getHours();
    var min=today.getMinutes();
    var sec=today.getSeconds();
    if(subName)
    {
      return name+'_'+subName+'_'+dd + '-' + mm + '-' + yyyy+'_'+hr + '-' + min + '-' + sec;
    }
    return name+'_'+dd + '-' + mm + '-' + yyyy+'_'+hr + '-' + min + '-' + sec;
  }

  // To bypass CORS issue, fileURL should be served through backend
  export const handleAgentDownload = async (fileUrl: string, fileName: string) => {
    try {
      const response = await fetch(fileUrl);
      const fileBlob = await response.blob();

      const downloadLink = document.createElement('a');
      downloadLink.href = URL.createObjectURL(fileBlob);
      downloadLink.download = fileName;
      downloadLink.click();

      URL.revokeObjectURL(downloadLink.href);
    } catch (error) {
      console.error('Error in downloading file:', error);
    }
  };

  export function formatBytes(bytes: number, decimals: number = 2) {
    if (bytes === 0) return "0 Bytes";

    const k = 1024;
    const dm = decimals < 0 ? 0 : decimals;
    const sizes = ["Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"];

    const i = Math.floor(Math.log(bytes) / Math.log(k));

    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + " " + sizes[i];
  }

  export function logout(){
    let username = localStorage.getItem("username");
    let password = localStorage.getItem("pswd");
    sessionStorage.removeItem("authenticatedUser");
    const fetchPromise = fetch(LOGOUT_DELETE_TOKEN_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ username, password }),
    });
    // console.log(fetchPromise);
    localStorage.removeItem("Token");
    localStorage.removeItem("Renew_access_token");
    localStorage.removeItem("Username");
    window.location.href = "/";
  }

  export function formatDuration(seconds: number): string {
    const days = Math.floor(seconds / (3600 * 24));
    const hours = Math.floor((seconds % (3600 * 24)) / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    const remainingSeconds = seconds % 60;
  
    const parts = [];
    if (days > 0) {
      parts.push(`${days} day${days > 1 ? 's' : ''}`);
    }
    if (hours > 0) {
      parts.push(`${hours} hour${hours > 1 ? 's' : ''}`);
    }
    if (minutes > 0) {
      parts.push(`${minutes} minute${minutes > 1 ? 's' : ''}`);
    }
    if (remainingSeconds > 0) {
      parts.push(`${remainingSeconds} second${remainingSeconds > 1 ? 's' : ''}`);
    }
  
    if (parts.length === 0) {
      return '0 seconds';
    } else if (parts.length === 1) {
      return parts[0];
    } else {
      const last = parts.pop();
      return `${parts.join(', ')} and ${last}`;
    }
}  

export const isValidPort = (portValue: string) => {
  if (/^\d{1,5}$/.test(portValue)) {
    // Check for single number
    const num = parseInt(portValue, 10);
    return num >= 0 && num <= 65536;
  } else if (/^\d{1,5}:\d{1,5}$/.test(portValue)) {
    // Check for the format "startNumber:endNumber"
    const [start, end] = portValue.split(":").map((num) => parseInt(num, 10));
    return (
      start >= 0 && end >= 0 && start <= 65536 && end <= 65536 && start < end
    );
  }
  return false;
};

export const isValidIp = (ipAddress: string) => {
  if (!ipAddress || ipAddress === "" || ipAddress === undefined) return false;
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
};