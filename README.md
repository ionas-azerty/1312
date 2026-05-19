# Vortex

Vortex is a modular desktop security monitoring application designed to detect suspicious tools, debuggers, DLL injectors, and suspicious behavior alongside FiveM and RAGE MP. It is built with a C++ backend, a React + Electron frontend, and a local Node.js API.

## Project Structure

- `client-ui/` — Electron + React dashboard UI and frontend resources.
- `core-scanner/` — C++ scanning engine modules and detection components.
- `driver-check/` — Kernel driver verification and system driver scan logic.
- `cloud-api/` — Local Node.js API and cloud sync gateway.
- `shared/` — Shared C++ utilities, configuration types, and data models.
- `logs/` — Runtime log exports and debug output.
- `config/` — Default configuration, signatures, and whitelist files.
- `main.cpp` — Vortex backend CLI application entrypoint.
- `CMakeLists.txt` — Build configuration for Windows-compatible C++ backend.

## Key Features

- Process scanner for suspicious tools such as Cheat Engine, x64dbg, Process Hacker.
- Modular detection engine with rule-based, signature, heuristic, and behavior modules.
- Memory and module inspection for suspicious DLLs, hooks, and RWX pages.
- Screenshot protection audit with overlay detection and window classification.
- Driver verification for unsigned or unusual kernel drivers.
- Live dashboard UI with dark theme, glassmorphism styling, and realtime logs.
- Local cloud sync API for signature updates and secure log export.
- Startup/tray support via Electron, notifications, and modern UI components.

## Build Instructions

### 1. Build the C++ backend

```bash
mkdir -p build
cd build
cmake ..
cmake --build . --config Release
```

The backend executable will be generated in `build/bin/vortex-backend`.

### 2. Install the local Node API dependencies

```bash
cd cloud-api
npm install
```

### 3. Install the client UI dependencies

```bash
cd ../client-ui
npm install
```

### 4. Run the application

First start the local API server:

```bash
cd cloud-api
npm start
```

Then start the frontend UI:

```bash
cd ../client-ui
npm run dev
```

If you want to run the Electron wrapper:

```bash
cd client-ui
npm run electron
```

## Local API Endpoints

- `GET /api/status` — Vortex API health
- `GET /api/scan` — Run live process scan
- `GET /api/driver-check` — Run driver audit
- `GET /api/screenshot` — Run screenshot overlay audit
- `GET /api/system` — Run anti-tamper and VM checks
- `POST /api/export-logs` — Export log snapshot

## Notes

- This project is strictly detection-focused. It does not inject code into games or implement cheats.
- The backend uses Windows APIs where available and includes cross-platform stubs for non-Windows builds.
- The UI is built with modern neon accent styling and a responsive dashboard layout.

## License

This project is delivered as a reference implementation for security monitoring and detection.
