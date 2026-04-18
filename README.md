# a templated based Phising Tool

SOCIAL ENGINEERING TEMPLATED BASED (SET-B)
written by: Muhammad Quwais Saputra

**this project is still in developing. hope you wait and help**

install
```bash
$ git clone https://github.com/mquwais-xcode/SET-B && bash build
```

run it
```bash
./set-b
```

you can select any templates you want by inputing the list code option like 0001.
after creating server running at http://localhost:5000, you can forward it using any port forwarding tools like SSH, Ngrok, LocalXpose, Cloudflare etc.
after your target input at your phising page, the raw will show to you
```bash
[*] Server running at http://0.0.0.0:5000
[*] Using Template: whatsapp

[!] VISITOR DEECTED!
 └── IP: 127.0.0.1

[!] WE GOT A JACKPOT! [whatsapp]
 └── Raw: password=568514
```

it is also can detect any visitor into your phising page. you can still use port forwarding tools but it always still detected new visitor and visitor input
