#Shadow Scan
A zero-dependency, hyper-optimized, cross-platform memory scanner + pattern sniffer written in C.
It doesn't ask for permissions. It doesn't log. It doesn't care about your OS.
It finds what it wants and leaves no trace.

🧠 Features
Cross‑platform – Windows, Linux, macOS (same codebase)

Zero dependencies – no external libs, no runtime frameworks

Hyper‑optimized – minimal overhead, fast scanning

Stealthy – no logging, no persistent artifacts

Low‑level memory access – uses native OS APIs (ntdll, /proc, vm_read)

Pattern sniffing – locate signatures and data in process memory

🚀 Compilation & Execution
Note: Administrative/root privileges are required to read other processes' memory.

Windows (MinGW or MSVC)
bash
gcc shadow_scan.c -o shadow_scan.exe -lpsapi -lntdll
./shadow_scan.exe
Linux (GCC)
bash
sudo gcc shadow_scan.c -o shadow_scan
sudo ./shadow_scan
macOS (Clang)
bash
clang shadow_scan.c -o shadow_scan
sudo ./shadow_scan
📦 Usage
text
./shadow_scan [options]
Option	Description
-p <pid>	Scan a specific process by PID
-n <name>	Scan a process by name (first match)
-s <signature>	Search for a byte pattern (hex string)
-h	Show help
Example – scan all processes for a pattern:

bash
sudo ./shadow_scan -s "48 8B 05 ?? ?? ?? ??"
⚠️ Disclaimer
This tool is intended for educational and research purposes only.
Unauthorized memory scanning may violate software terms of service or local laws.
Use it only on systems you own or have explicit permission to test.
The author assumes no liability for misuse.

📄 License
This project is released under the MIT License – see the LICENSE file for details.

🤝 Contributing
Pull requests, issues, and feature requests are welcome.
For major changes, please open an issue first to discuss what you would like to change.

Built with ❤️ for the curious minds.
