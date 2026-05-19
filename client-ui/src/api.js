const API_ROOT = 'http://127.0.0.1:4000/api';

async function request(path, options = {}) {
  const response = await fetch(`${API_ROOT}${path}`, {
    headers: { 'Content-Type': 'application/json' },
    ...options
  });
  return response.json();
}

export async function getStatus() {
  return request('/status');
}

export async function runLiveScan() {
  return request('/scan');
}

export async function runDriverCheck() {
  return request('/driver-check');
}

export async function runScreenshotAudit() {
  return request('/screenshot');
}

export async function runSystemCheck() {
  return request('/system');
}

export async function exportLogs() {
  return request('/export-logs', { method: 'POST' });
}
