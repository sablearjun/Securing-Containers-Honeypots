import React from "react";
import Tab from "react-bootstrap/Tab";
import Tabs from "react-bootstrap/Tabs";
import { AGENT_INSTALLER_URL } from "../apis/urls";

const DownloadAgent = () => {
  return (
    <>
      <div className="card py-3 vh-75">
        <div className="card-header bg-white border-0 mb-3">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Download Agent</h4>
            </div>
          </div>
        </div>

        <Tabs
          defaultActiveKey="windows"
          id="uncontrolled-tab-example"
          className="mb-3 text-primary"
        >
          <Tab
            eventKey="windows"
            title={
              <span className="fs-4">
                <i className="fa fa-windows"></i> &nbsp; Windows
              </span>
            }
          >
            <div className="p-7">
              <div className="d-flex align-items-start flex-column">
                <div className="mb-5">
                  <a href={AGENT_INSTALLER_URL} className="btn btn-outline-primary">
                    <i className="fa fa-download"></i> &nbsp;Download Now
                  </a>
                </div>
                <div className="d-flex align-items-start col-12 flex-column flex-lg-row">
                  <div
                    className="nav flex-column nav-pills me-3"
                    id="v-pills-tab"
                    role="tablist"
                    aria-orientation="vertical"
                  >
                    <button
                      className="nav-link active btn-primary p-3"
                      id="v-pills-install-tab"
                      data-bs-toggle="pill"
                      data-bs-target="#v-pills-install"
                      type="button"
                      role="tab"
                      aria-controls="v-pills-install"
                      aria-selected="true"
                    >
                      Installation Guide
                    </button>
                    <button
                      className="nav-link btn-primary p-3"
                      id="v-pills-uninstall-tab"
                      data-bs-toggle="pill"
                      data-bs-target="#v-pills-uninstall"
                      type="button"
                      role="tab"
                      aria-controls="v-pills-uninstall"
                      aria-selected="false"
                    >
                      Uninstallation Guide
                    </button>
                    <button
                      className="nav-link btn-primary p-3"
                      id="v-pills-troubleshoot-tab"
                      data-bs-toggle="pill"
                      data-bs-target="#v-pills-troubleshoot"
                      type="button"
                      role="tab"
                      aria-controls="v-pills-troubleshoot"
                      aria-selected="false"
                    >
                      Troubleshooting
                    </button>
                  </div>
                  <div className="tab-content col-12 col-lg-6 m-5" id="v-pills-tabContent">
                    <div
                      className="tab-pane fade show active col-12"
                      id="v-pills-install"
                      role="tabpanel"
                      aria-labelledby="v-pills-install-tab"
                    >
                      <div className="mx-3 d-flex col-10 flex-column flex-lg-row">
                        <div className="p-3 col-12 bg-info-light mx-3">
                          <h2 className="text-primary fs-4 ms-2 mb-3">
                            Installation Guide
                          </h2>
                          <p className="fst-italic text-primary fw-light">Prerequiste: Administrator Access is necessary for installing Vajra-Client on Windows</p>
                          <p>
                            <ol>
                              <li className="fw-semi">Download the Vajra Installer</li>
                                <ul>
                                  <li>Click on <a href={AGENT_INSTALLER_URL} className="text-decoration-none text-primary">"Download Now"</a> to start the download process.</li>
                                </ul>
                              <li className="fw-semi">Unzip the folder</li>
                                <ul>
                                  <li>Locate the downloaded ZIP file on your computer.</li>
                                  <li>Right-click the ZIP file and select "Extract All" to unzip the file.</li>
                                  <li>Choose a destination folder where you want to extract the vajra agent files.</li>
                                </ul>
                              <li className="fw-semi">Run Installer Script as Administrator</li>
                                <ul>
                                  <li>Inside the extracted "vajra-client/windows" folder, you should find a file named "Vajra_install.bat."</li>
                                  <li>Right-click on "Vajra_install.bat" to open the context menu.</li>
                                  <li>Select "Run as Administrator." This is important to ensure that the installation process has the necessary permissions to make system-level changes if required.</li>
                                </ul>
                              <li className="fw-semi">Completion</li>
                                <ul>
                                  <li>Once you've run the installation script with administrative privileges, it will execute the installation process.</li>
                                  <li>Follow any on-screen prompts or instructions provided by the installation script.</li>
                                  <li>When the installation is complete, you should see a confirmation message or indication that the "vajra agent" has been successfully installed.</li>
                                </ul>
                            </ol>
                          </p>
                          <p>That's it! You have now successfully installed the "vajra agent" on your computer. You can proceed to view the details on this website as needed.</p>
                        </div>
                        <iframe className="col-12" height="300"
                        src="https://www.youtube.com/embed/Y5Ivx9VoSTk">
                        </iframe>
                      </div>
                    </div>
                    <div
                      className="tab-pane fade"
                      id="v-pills-uninstall"
                      role="tabpanel"
                      aria-labelledby="v-pills-uninstall-tab"
                    >
                      <div className="mx-3 d-flex col-10 flex-column flex-lg-row">
                        <div className="p-3 col-12 bg-info-light mx-3">
                          <h2 className="text-primary fs-4 ms-2 mb-3">
                            Uninstallation Guide
                          </h2>
                          <p className="fst-italic text-primary fw-light">Note: Skip over to step 3 if you already have the vajra-client extracted zip</p>
                          <p>
                            <ol>
                              <li className="fw-semi">Download the Vajra Uninstaller</li>
                                <ul>
                                  <li>Click on <a href={AGENT_INSTALLER_URL} className="text-decoration-none text-primary">"Download Now"</a> to start the download process.</li>
                                </ul>
                              <li className="fw-semi">Unzip the folder</li>
                                <ul>
                                  <li>Locate the downloaded ZIP file on your computer.</li>
                                  <li>Right-click the ZIP file and select "Extract All" to unzip the file.</li>
                                  <li>Choose a destination folder where you want to extract the vajra agent files.</li>
                                </ul>
                              <li className="fw-semi">Run Uninstaller Script as Administrator</li>
                                <ul>
                                  <li>Inside the extracted "vajra-client/windows" folder, you should find a file named "Vajra_uninstall.bat."</li>
                                  <li>Right-click on "Vajra_uninstall.bat" to open the context menu.</li>
                                  <li>Select "Run as Administrator." This is important to ensure that the uninstallation process has the necessary permissions to make system-level changes if required.</li>
                                </ul>
                              <li className="fw-semi">Completion</li>
                                <ul>
                                  <li>Once you've run the uninstallation script with administrative privileges, it will execute the uninstallation process.</li>
                                  <li>Follow any on-screen prompts or instructions provided by the uninstallation script.</li>
                                  <li>When the uninstallation is complete, you should see a confirmation message or indication that the "vajra agent" has been successfully uninstalled.</li>
                                </ul>
                            </ol>
                          </p>
                          <p>That's it! You have now successfully uninstalled the "vajra agent" on your computer.</p>
                        </div>
                        <iframe className="col-12" height="300"
                        src="https://www.youtube.com/embed/DxR4lkSIXM4">
                        </iframe>
                      </div>
                    </div>
                    <div
                      className="tab-pane fade"
                      id="v-pills-troubleshoot"
                      role="tabpanel"
                      aria-labelledby="v-pills-troubleshoot-tab"
                    >
                      <div className="mx-3 bg-info-light">
                        <div className="p-3">
                          <h2 className="text-primary fs-4 ms-2 mb-3">
                            Troubleshooting Guide
                          </h2>
                          <p>
                            <span className="ms-3 my-2">
                            Here's an elaboration of the troubleshooting steps if Node is offline:
                            </span>
                            <ol>
                              <li className="fw-semi">Check Vajra Service</li>
                                <ul>
                                  <li>Press the Windows key on your keyboard.</li>
                                  <li>In the Windows search bar, type "services" and press Enter. This will open the Windows Services application.</li>
                                  <li>Look for a service named "Vajra service" in the list of services.</li>
                                  <li>If you find "Vajra service" and it is listed as "Stopped," it means the service is not running.</li>
                                  <li>Right-click on "Vajra service" and select "Start" from the context menu. This will initiate the service and bring the Node online.</li>
                                </ul>
                              <li className="fw-semi">Check PLGX Extension Service</li>
                                <ul>
                                  <li>In the same Windows Services application, look for a service named "plgx_extension service."</li>
                                  <li>If you find "plgx_extension service" and it is listed as "Stopped," it means the service is not running.</li>
                                  <li>Right-click on "plgx_extension service" and select "Start" from the context menu. This will initiate the service and ensure its proper functioning.</li>
                                </ul>
                              <li className="fw-semi">Verify Network Connectivity</li>
                                <ul>
                                  <li>Ensure that your client machine (the one running the "vajra-client") has a stable and active network connection.</li>
                                  <li>Check if the client can successfully connect to the server where Vajra is hosted.</li>
                                  <li>You can test network connectivity by trying to access the server using its IP address or hostname.</li>
                                  <li>If there are any issues with network connectivity, resolve them to ensure that the client can communicate with the server.</li>
                                </ul>
                            </ol>
                          </p>
                          <p>
                          By following these steps, you can troubleshoot and resolve issues with Node being offline on Web UI. Restarting the required services and verifying network connectivity should help bring Node back online and ensure the proper functioning of the Vajra agent.
                          </p>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </Tab>
          <Tab
            eventKey="linux"
            title={
              <span className="fs-4">
                <i className="fa fa-linux"></i> &nbsp; Linux
              </span>
            }
          >
            <div className="p-7">
              <div className="d-flex align-items-start flex-column">
                <div className="mb-5">
                  <a href={AGENT_INSTALLER_URL} className="btn btn-outline-primary">
                    <i className="fa fa-download"></i> &nbsp;Download Now
                  </a>
                </div>
                <div className="d-flex align-items-start col-12 flex-column flex-lg-row">
                  <div
                    className="nav flex-column nav-pills me-3"
                    id="v-l-pills-tab"
                    role="tablist"
                    aria-orientation="vertical"
                  >
                    <button
                      className="nav-link active btn-primary p-3"
                      id="v-l-pills-install-tab"
                      data-bs-toggle="pill"
                      data-bs-target="#v-l-pills-install"
                      type="button"
                      role="tab"
                      aria-controls="v-l-pills-install"
                      aria-selected="true"
                    >
                      Installation Guide
                    </button>
                    <button
                      className="nav-link btn-primary p-3"
                      id="v-l-pills-uninstall-tab"
                      data-bs-toggle="pill"
                      data-bs-target="#v-l-pills-uninstall"
                      type="button"
                      role="tab"
                      aria-controls="v-l-pills-uninstall"
                      aria-selected="false"
                    >
                      Uninstallation Guide
                    </button>
                    <button
                      className="nav-link btn-primary p-3"
                      id="v-l-pills-troubleshoot-tab"
                      data-bs-toggle="pill"
                      data-bs-target="#v-l-pills-troubleshoot"
                      type="button"
                      role="tab"
                      aria-controls="v-l-pills-troubleshoot"
                      aria-selected="false"
                    >
                      Troubleshooting
                    </button>
                  </div>
                  <div className="tab-content col-12 col-md-6 m-5 m-md-0" id="v-pills-tabContent">
                    <div
                      className="tab-pane fade show active"
                      id="v-l-pills-install"
                      role="tabpanel"
                      aria-labelledby="v-l-pills-install-tab"
                    >
                      <div className="mx-3 bg-info-light flex-column flex-lg-row">
                        <div className="p-3">
                          <h2 className="text-primary fs-4 ms-2 mb-3">
                            Installation Guide
                          </h2>
                          <p className="fst-italic text-primary fw-light">Prerequiste: Root Access is necessary for installing Vajra-Client on Linux</p>
                          <p>
                            <ol>
                              <li className="fw-semi">Download the Vajra Installer</li>
                                <ul>
                                  <li>Click on <a href={AGENT_INSTALLER_URL} className="text-decoration-none text-primary">"Download Now"</a> to start the download process.</li>
                                </ul>
                              <li className="fw-semi">Unzip the folder</li>
                                <ul>
                                  <li>Locate the downloaded ZIP file on your computer.</li>
                                  <li>Right-click the ZIP file and select "Extract All" to unzip the file.</li>
                                  <li>Choose a destination folder where you want to extract the vajra agent files.</li>
                                </ul>
                              <li className="fw-semi">Locate the vajrainstall.sh file</li>
                                <ul>
                                  <li>Launch the Terminal application on your computer. You can typically find the Terminal in the "Applications" section or by searching for "Terminal" on Linux.</li>
                                  <li>Use the cd command in the Terminal to navigate to the directory where you extracted the "vajra-client" file. For example:
                                   <p className="mt-3"> <span className="bg-black bg-opacity-75 text-white p-2 m-2">cd /path/to/vajra-client/linux</span></p></li>
                                </ul>
                              <li className="fw-semi">Change File Permissions:</li>
                                <ul>
                                  <li>Execute the following command to change the permissions of the "vajrainstall.sh" file to make it executable:
                                  <p className="mt-3"><span className="bg-black bg-opacity-75 text-white p-2 m-2">chmod +x vajrainstall.sh</span></p>
                                  </li>
                                </ul>
                              <li className="fw-semi">Run Installer Script</li>
                                <ul>
                                  <li>Run the installation script with elevated privileges using the sudo command. This is done to ensure that the installation process has the necessary permissions to make system-level changes:
                                  <p className="mt-3"><span className="bg-black bg-opacity-75 text-white p-2 m-2">sudo ./vajrainstall.sh</span></p>
                                  </li>
                                </ul>
                              <li className="fw-semi">Completion</li>
                                <ul>
                                  <li>The installation script will execute, and you may be prompted to provide your system password, as it requires administrative privileges.</li>
                                  <li>Follow any on-screen prompts or instructions provided by the installation script.</li>
                                  <li>Once the installation script completes its tasks, you will see a confirmation message indicating that the installation is finished.</li>
                                </ul>
                            </ol>
                          </p>
                          <p>That's it! You have now successfully installed the "vajra agent" on your computer. You can proceed to view the details on this website as needed.</p>
                        </div>
                      </div>
                    </div>
                    <div
                      className="tab-pane fade"
                      id="v-l-pills-uninstall"
                      role="tabpanel"
                      aria-labelledby="v-l-pills-uninstall-tab"
                    >
                      <div className="mx-3 bg-info-light flex-column flex-lg-row">
                        <div className="p-3">
                          <h2 className="text-primary fs-4 ms-2 mb-3">
                            Uninstallation Guide
                          </h2>
                          <p className="fst-italic text-primary fw-light">Note: Skip over to step 3 if you already have the vajra-client extracted zip</p>
                          <p>
                            <ol>
                              <li className="fw-semi">Download the Vajra Installer</li>
                                <ul>
                                  <li>Click on <a href={AGENT_INSTALLER_URL} className="text-decoration-none text-primary">"Download Now"</a> to start the download process.</li>
                                </ul>
                              <li className="fw-semi">Unzip the folder</li>
                                <ul>
                                  <li>Locate the downloaded ZIP file on your computer.</li>
                                  <li>Right-click the ZIP file and select "Extract All" to unzip the file.</li>
                                  <li>Choose a destination folder where you want to extract the vajra agent files.</li>
                                </ul>
                              <li className="fw-semi">Locate the vajrainstall.sh file</li>
                                <ul>
                                  <li>Launch the Terminal application on your computer. You can typically find the Terminal in the "Applications" section or by searching for "Terminal" on Linux.</li>
                                  <li>Use the cd command in the Terminal to navigate to the directory where you extracted the "vajra-client" file. For example:
                                   <p className="mt-3"> <span className="bg-black bg-opacity-75 text-white p-2 m-2">cd /path/to/vajra-client/linux</span></p></li>
                                </ul>
                              <li className="fw-semi">Change File Permissions:</li>
                                <ul>
                                  <li>Execute the following command to change the permissions of the "vajrainstall.sh" file to make it executable:
                                  <p className="mt-3"><span className="bg-black bg-opacity-75 text-white p-2 m-2">chmod +x vajrainstall.sh</span></p>
                                  </li>
                                </ul>
                              <li className="fw-semi">Run Uninstaller Script</li>
                                <ul>
                                  <li>Run the installation script with "uninstall flag" with elevated privileges using the sudo command. This is done to ensure that the uninstallation process has the necessary permissions to make system-level changes:
                                  <p className="mt-3"><span className="bg-black bg-opacity-75 text-white p-2 m-2">sudo ./vajrauninstall.sh -uninstall</span></p>
                                  </li>
                                </ul>
                              <li className="fw-semi">Completion</li>
                                <ul>
                                  <li>The uninstallation script will execute, and you may be prompted to provide your system password, as it requires administrative privileges.</li>
                                  <li>Follow any on-screen prompts or instructions provided by the uninstallation script.</li>
                                  <li>Once the installation script completes its tasks, you will see a confirmation message indicating that the uninstallation is finished.</li>
                                </ul>
                            </ol>
                          </p>
                          <p>That's it! You have now successfully uninstalled the "vajra agent" on your computer.</p>
                        </div>
                      </div>
                    </div>
                    <div
                      className="tab-pane fade"
                      id="v-l-pills-troubleshoot"
                      role="tabpanel"
                      aria-labelledby="v-l-pills-troubleshoot-tab"
                    >
                      <div className="mx-3 bg-info-light">
                        <div className="p-3">
                          <h2 className="text-primary fs-4 ms-2 mb-3">
                            Troubleshooting Guide
                          </h2>
                          <p>
                            <span className="ms-3 my-2">
                              Here's an elaboration of the troubleshooting steps if Node is offline:
                            </span>
                            <ol>
                              <li className="fw-semi">Check Vajra Service Status:</li>
                                <ul>
                                  <li>Open a terminal window on your Linux machine.</li>
                                  <li>Use the following command to check the status of the Vajra service:
                                    <p className="mt-3"><span className="bg-black bg-opacity-75 text-white p-2 m-2">sudo systemctl status vajra.service</span></p>
                                  </li>
                                  <li>This command will provide information about whether the Vajra service is running or not. Look for the "Active" status. If it's in "active" or "running" mode, the service is working correctly.</li>
                                  <li>If the service is not running, you can start it using the following command:
                                  <p className="mt-3"><span className="bg-black bg-opacity-75 text-white p-2 m-2">sudo systemctl start vajra.service</span></p>
                                  </li>
                                </ul>
                              <li className="fw-semi">Check client`s network connectivity with server</li>
                                <ul>
                                  <li>To ensure that the Vajra client can communicate with the server, you need to check the network connectivity.</li>
                                  <li>Verify that your Linux machine is connected to the network, and you have internet access. You can check this by opening a web browser and trying to access a website.</li>
                                  <li>Next, confirm that the server hosting the Vajra service is reachable from your Linux machine. You can use the ping command followed by the server's IP address or hostname:
                                    <p className="mt-3"><span className="bg-black bg-opacity-75 text-white p-2 m-2">ping getvajra.com</span></p>
                                  </li>
                                  <li>If you receive replies from the server, it means your client can communicate with the server. If there are issues, you may need to check your network configuration, firewall settings, or server availability.</li>
                                </ul>
                            </ol>
                          </p>
                          <p>By following these troubleshooting steps, you can determine whether the Vajra service is running, ensure that your Linux client has proper network connectivity to the server and address any issues found to ensure the smooth operation of Vajra.</p>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </Tab>
        </Tabs>
      </div>
    </>
  );
};

export default DownloadAgent;
