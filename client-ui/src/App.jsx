import { useEffect, useState } from 'react';
import { getStatus, runLiveScan, runDriverCheck, runScreenshotAudit, runSystemCheck, exportLogs } from './api';

const sections = [
  'Dashboard',
  'Live Scan',
  'Drivers',
  'Detections',
  'Settings',
  'Logs'
];

const createCard = (title, subtitle, value) => (
  <div className="card" key={title}>
    <div className="card-title">{title}</div>
    <div className="card-value">{value}</div>
    <div className="card-subtitle">{subtitle}</div>
  </div>
);

function App() {
  const [activeSection, setActiveSection] = useState('Dashboard');
  const [status, setStatus] = useState({ message: 'Starting Vortex engine...' });
  const [results, setResults] = useState([]);
  const [logs, setLogs] = useState([]);

  useEffect(() => {
    getStatus().then(setStatus).catch(() => setStatus({ message: 'Local API unavailable' }));
  }, []);

  const refreshScan = async () => {
    const scan = await runLiveScan();
    setResults(scan.results || []);
    setLogs(prev => [{ timestamp: new Date().toISOString(), category: 'Scan', message: 'Live scan completed' }, ...prev]);
  };

  const refreshDrivers = async () => {
    const scan = await runDriverCheck();
    setResults(scan.drivers || []);
    setLogs(prev => [{ timestamp: new Date().toISOString(), category: 'Driver', message: 'Driver audit completed' }, ...prev]);
  };

  const refreshSystem = async () => {
    const scan = await runSystemCheck();
    setResults(scan.system || []);
    setLogs(prev => [{ timestamp: new Date().toISOString(), category: 'System', message: 'System check completed' }, ...prev]);
  };

  const auditScreenshot = async () => {
    const scan = await runScreenshotAudit();
    setResults(scan.screenshots || []);
    setLogs(prev => [{ timestamp: new Date().toISOString(), category: 'Screenshot', message: 'Overlay audit completed' }, ...prev]);
  };

  return (
    <div className="app-shell">
      <aside className="sidebar">
        <div className="brand">Vortex</div>
        <div className="brand-subtitle">Threat Observer</div>
        <div className="nav-list">
          {sections.map(section => (
            <button key={section} className={`nav-button ${activeSection === section ? 'active' : ''}`} onClick={() => setActiveSection(section)}>
              {section}
            </button>
          ))}
        </div>
      </aside>
      <main className="content">
        <header className="topbar">
          <div>
            <div className="headline">{activeSection}</div>
            <div className="subline">{status.message || 'Connected to the local Vortex API.'}</div>
          </div>
          <div className="stats-panel">
            {createCard('CPU', 'Live usage', '18%')}
            {createCard('RAM', 'Memory pressure', '3.4 GB')}
          </div>
        </header>

        <section className="grid-panel">
          {createCard('Detection Engine', 'Rule, signature, heuristic, behavior', 'Online')}
          {createCard('Scan History', 'Recent activity snapshot', results.length.toString())}
          {createCard('Driver Monitor', 'Kernel driver audit', 'Stable')}
        </section>

        <section className="actions-panel">
          <button onClick={refreshScan}>Run Live Scan</button>
          <button onClick={refreshDrivers}>Check Drivers</button>
          <button onClick={auditScreenshot}>Screenshot Audit</button>
          <button onClick={refreshSystem}>Anti-Tamper Check</button>
          <button onClick={exportLogs}>Export Logs</button>
        </section>

        <section className="results-panel">
          <div className="section-title">Latest detections</div>
          {results.length === 0 ? (
            <div className="empty-state">No suspicious activity detected yet.</div>
          ) : (
            <div className="result-list">
              {results.map((item, index) => (
                <div className="result-card" key={index}>
                  <div className="result-row">
                    <strong>{item.processName || item.path}</strong>
                    <span className={`tag ${item.riskLevel}`}>{item.riskLevel}</span>
                  </div>
                  <div className="result-text">{item.reason}</div>
                  <div className="result-meta">{item.timestamp}</div>
                </div>
              ))}
            </div>
          )}
        </section>

        <section className="logs-panel">
          <div className="section-title">Activity log</div>
          <div className="log-list">
            {logs.map((entry, index) => (
              <div className="log-entry" key={index}>
                <span>{entry.timestamp}</span>
                <strong>{entry.category}</strong>
                <p>{entry.message}</p>
              </div>
            ))}
          </div>
        </section>
      </main>
    </div>
  );
}

export default App;
