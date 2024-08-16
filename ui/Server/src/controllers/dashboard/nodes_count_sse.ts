import { Request, Response, NextFunction } from 'express';
import { dbSourceConnection } from 'index';

let clients: Response[] = []; // Array to store connected clients
let previousNodes: { Online: number; Offline: number; Total: number } = { Online: 0, Offline: 0, Total: 0 };
let intervalId: NodeJS.Timeout | null = null; // Store the interval ID

export const nodes_count_sse = async (req: Request, res: Response, next: NextFunction) => {
  res.writeHead(200, {
    "Content-Type": "text/event-stream",
    "Cache-Control": "no-cache",
    "Connection": "keep-alive"
  });

  // Add the response object to the clients array when a new connection is established
  clients.push(res);
  res.write(`data: ${JSON.stringify(previousNodes)}\n\n`);
  //  console.log("Client Size:", clients.length);
  req.on('close', () => {
    // Remove the response object from the clients array when the connection is closed
    const index = clients.indexOf(res);
    if (index !== -1) {
      clients.splice(index, 1);
    }

    // Clear the interval when the request is closed
    if (intervalId !== null) {
      clearInterval(intervalId);
      intervalId = null; // Reset the interval ID
    }
    // clients = clients.filter((i,y) => y!=index);
  });

  // Function to send updates to the client
  const sendUpdatesAtIntervals = () => {
    const INTERVAL = 5 * 1000; // Interval in milliseconds (e.g., every 5 seconds)
    intervalId= setInterval(async () => {
      try {
        const nodesRepository = dbSourceConnection()[1];
        const nodesResult = await nodesRepository.query('SELECT last_seen from nodes');

        let online = 0;
        let offline = 0;
        let diff: number;
        let currentTime = Math.floor(Date.now() / 1000);
        const LIMIT: number = 60; // seconds

        for (let i = 0; i < nodesResult.length; i++) {
          diff = currentTime - nodesResult[i].last_seen;
          if (diff < LIMIT) {
            online++;
          } else {
            offline++;
          }
        }

        const nodes = {
          Online: online,
          Offline: offline,
          Total: online + offline,
        };

        // Compare with the previous state
        if (
          nodes.Online !== previousNodes.Online ||
          nodes.Offline !== previousNodes.Offline ||
          nodes.Total !== previousNodes.Total
        ) {
          // Send updates to all connected clients
          for (const client of clients) {
            client.write(`data: ${JSON.stringify(nodes)}\n\n`);
          }
          // Update the previousNodes state
          console.log("Previous Nodes: ", previousNodes);
          console.log("Nodes: ", nodes);
          previousNodes = nodes;
        }
        console.log("TimeStamp: ", new Date(Date.now()));
        console.log("Client Length: ", clients.length);
      } catch (err) {
        console.error('Error while sending SSE update:', err);
      }
    }, INTERVAL);
  };

  // Initially, send the current state to the client
  sendUpdatesAtIntervals();
};
