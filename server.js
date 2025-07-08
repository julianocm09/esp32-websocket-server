const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: process.env.PORT || 3000 });

let clients = [];

wss.on('connection', function connection(ws) {
  console.log('Dispositivo conectado');

  clients.push(ws);

  ws.on('message', function incoming(message) {
    console.log('Recebido:', message);

    clients.forEach(client => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(`ESP32 respondeu: ${message}`);
      }
    });
  });

  ws.on('close', () => {
    console.log('Dispositivo desconectado');
    clients = clients.filter(client => client !== ws);
  });

  ws.send('Servidor: Conexão estabelecida');
});

console.log('Servidor WebSocket rodando...');
