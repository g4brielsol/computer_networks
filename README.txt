Como executar os Makefiles:
    Clique com o botão direito do mouse em cima da pasta Server -> Abrir no Terminal. No terminal, digite "make all" sem aspas,
    depois "make run" também sem aspas. O server vai utilizar a porta 9000, qualquer problema é só alterar a porta no makefile.

    Depois siga os mesmos passos para a pasta Client, clique com o botão direito do mouse na pasta -> Abrir no Terminal. 
    Digite "make all" no terminal sem aspas, depois "make run" também sem aspas. O cliente vai utilizar a porta 9000 e o IP 127.0.0.1,
    qualquer problema é só alterá-los no makefile.

    Após isso, digite uma mensagem no terminal do Client, a mensagem aparecerá no terminal do Server, responda no Server e a mensagem
    será mostrada no terminal do Client e assim por diante. Como esse é só o módulo um, se for digitado mais de uma mensagem por terminal
    ou se a primeira mensagem for mandada pelo terminal do Server, ocorrerá um problema de ordenação dos textos, isso será corrigido nos 
    próximos módulos para os chats de múltiplos clientes.

    Para fechar a comunicação, digite a palavra "quit" sem as aspas em qualquer terminal.

