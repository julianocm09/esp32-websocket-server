const WebSocket = require('ws');

const TOKENS_VALIDOS = ['JCM-001', 'JCM-ABC', 'JCM-12345', 'JCM-destop', 'ESP324676EFD0', 'paineljuliano'];

const wss = new WebSocket.Server({
  port: process.env.PORT || 3000
});

let clients = [];

wss.on('connection', function connection(ws) {
  ws.isAuthorized = false;

  console.log('🔌 Nova conexão aguardando token');

  ws.on('message', function incoming(message) {
    // Se não autorizado, aguarda o token
    if (!ws.isAuthorized) {
      let token;
      try {
        const data = JSON.parse(message.toString());
        token = data.token;
      } catch (err) {
        ws.send('Erro: formato inválido, envie {"token":"seu_token"}');
        ws.close(1008, 'Formato inválido');
        return;
      }

      if (TOKENS_VALIDOS.includes(token)) {
        ws.isAuthorized = true;
        ws.token = token;
        clients.push(ws);
        ws.send('Servidor: Token válido. Conexão autorizada.');
        console.log(`✅ Cliente autorizado com token: ${token}`);
      } else {
        ws.send('Erro: Token inválido');
        ws.close(1008, 'Token inválido');
        console.log('❌ Conexão recusada: Token inválido');
      }
      return;
    }

    // Cliente autorizado, repassa mensagem como texto
    const textMessage = message.toString(); // 🔧 Força envio como texto
    console.log('📩 Recebido:', textMessage);

    clients.forEach(client => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(textMessage);
      }
    });
  });

  ws.on('close', (code, reason) => {
    console.log(`🔌 Cliente desconectado - Código: ${code}, Motivo: ${reason.toString()}`);
    clients = clients.filter(client => client !== ws);
  });
});

console.log('🚀 Servidor WebSocket rodando na porta 3000...');
