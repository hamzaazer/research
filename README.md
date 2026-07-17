# 🔍 Shadow Scan

**A zero‑dependency, hyper‑optimized, cross‑platform memory scanner + pattern sniffer written in C.**  
It doesn't ask for permissions. It doesn't log. It doesn't care about your OS.  
It finds what it wants and leaves no trace.

---

## ✨ Features

- 🌍 **Cross‑platform** – Windows, Linux, macOS (same codebase)
- 🚀 **Zero dependencies** – no external libs, no runtime frameworks
- ⚡ **Hyper‑optimized** – minimal overhead, fast scanning
- 🕵️ **Stealthy** – no logging, no persistent artifacts
- 🧠 **Low‑level memory access** – uses native OS APIs (`ntdll`, `/proc`, `mach_vm`)
- 🎯 **Pattern sniffing** – locate byte signatures with wildcards (`??`)

---

## 📦 Compilation & Execution

> **Note:** Administrative/root privileges are required to read other processes' memory.

| Platform | Command |
|----------|---------|
| **Windows** (MinGW/MSVC) | `gcc shadow_scan.c -o shadow_scan.exe -lpsapi -lntdll`<br>`./shadow_scan.exe` |
| **Linux** (GCC) | `sudo gcc shadow_scan.c -o shadow_scan`<br>`sudo ./shadow_scan` |
| **macOS** (Clang) | `clang shadow_scan.c -o shadow_scan`<br>`sudo ./shadow_scan` |

---

## 🎮 Usage

```
./shadow_scan -p <pid> -s "<pattern>"
./shadow_scan -n <name> -s "<pattern>"
./shadow_scan -h
```

| Option | Description |
|--------|-------------|
| `-p <pid>` | Scan a specific process by PID |
| `-n <name>` | Scan a process by name (first match) |
| `-s <pattern>` | Search for a byte pattern (hex string with spaces, `??` for wildcard) |
| `-h` | Show help |

---

## 📝 Examples

**1. Scan a process by name for a pattern:**

```bash
sudo ./shadow_scan -n "chrome" -s "48 8B 05 ?? ?? ?? ??"
```

**2. Scan a specific PID:**

```bash
sudo ./shadow_scan -p 1234 -s "FF 25 ?? ?? ?? ??"
```

**3. Show help:**

```bash
./shadow_scan -h
```

---

## 🧩 Pattern Format

- Bytes are written as **hexadecimal**, separated by spaces.
- Use `??` as a **wildcard** for any single byte.

**Example:**  
`48 8B 05 ?? ?? ?? ??`  
matches `48 8B 05 12 34 56 78` or `48 8B 05 AB CD EF 01`, etc.

---

## 🛠 How It Works

1. **Process enumeration** – find target by name or PID.
2. **Memory region discovery** – enumerate all readable memory areas.
3. **Chunked reading** – read memory in blocks (with overlap to handle cross‑boundary matches).
4. **Pattern matching** – fast byte‑by‑byte comparison with wildcard support.
5. **Output** – prints addresses where the pattern is found.

---

## ⚠️ Disclaimer

> This tool is intended for **educational and research purposes only**.  
> Unauthorized memory scanning may violate software terms of service or local laws.  
> Use it only on systems you own or have explicit permission to test.  
> The author assumes no liability for misuse.

---

## 📄 License

This project is released under the **MIT License** – see the [LICENSE](LICENSE) file for details.

---

## 🤝 Contributing

Pull requests, issues, and feature requests are welcome.  
For major changes, please open an issue first to discuss what you would like to change.

---

**Built with ❤️ for the curious minds.**
