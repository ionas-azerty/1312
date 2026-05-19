import express from 'express';
import cors from 'cors';
import path from 'path';
import { fileURLToPath } from 'url';
import { spawnFile } from 'child_process';
import fs from 'fs';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const configPath = path.join(__dirname, 'config', 'local.config.json');
const config = JSON.parse(fs.readFileSync(configPath, 'utf8'));
const app = express();

app.use(cors({ origin: config.allowedOrigins }));
app.use(express.json());

function resolveBackend() {
  const defaultPath = path.resolve(__dirname, config.backendPath);
  if (fs.existsSync(defaultPath)) {
    return defaultPath;
  }
  const fallback = path.resolve(__dirname, '../build/bin/vortex-backend');
  return fs.existsSync(fallback) ? fallback : null;
}

function executeBackend(args) {
  return new Promise((resolve, reject) => {
    const backend = resolveBackend();
    if (!backend) {
      return reject(new Error('Vortex backend executable not found'));
    }
    const process = spawnFile(backend, args, { cwd: path.dirname(backend) });
    let output = '';
    process.stdout.on('data', data => (output += data.toString()));
    process.stderr.on('data', data => (output += data.toString()));
    process.on('close', code => {
      if (code !== 0) {
        return reject(new Error(output));
      }
      try {
        resolve(JSON.parse(output));
      } catch (error) {
        resolve({ status: 'ok', raw: output });
      }
    });
  });
}

app.get('/api/status', async (req, res) => {
  res.json({ status: 'ready', message: 'Vortex local API is online' });
});

app.get('/api/scan', async (req, res) => {
  try {
    res.json(await executeBackend(['--scan']));
  } catch (err) {
    res.status(500).json({ status: 'error', message: err.message });
  }
});

app.get('/api/driver-check', async (req, res) => {
  try {
    res.json(await executeBackend(['--driver-check']));
  } catch (err) {
    res.status(500).json({ status: 'error', message: err.message });
  }
});

app.get('/api/screenshot', async (req, res) => {
  try {
    res.json(await executeBackend(['--screenshot']));
  } catch (err) {
    res.status(500).json({ status: 'error', message: err.message });
  }
});

app.get('/api/system', async (req, res) => {
  try {
    res.json(await executeBackend(['--status']));
  } catch (err) {
    res.status(500).json({ status: 'error', message: err.message });
  }
});

app.post('/api/export-logs', (req, res) => {
  const output = path.resolve(__dirname, '../logs/vortex-export.log');
  fs.writeFileSync(output, `${new Date().toISOString()} - logs exported\n`, 'utf8');
  res.json({ status: 'ok', path: output });
});

app.listen(config.port, () => {
  console.log(`Vortex local API listening on port ${config.port}`);
});
