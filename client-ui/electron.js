import { app, BrowserWindow, Tray, Menu, nativeImage, Notification } from 'electron';
import path from 'path';
import url from 'url';

let mainWindow;
let tray;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1360,
    height: 820,
    minWidth: 1000,
    minHeight: 660,
    show: false,
    frame: false,
    transparent: true,
    webPreferences: {
      preload: path.join(process.cwd(), 'preload.js'),
      nodeIntegration: false,
      contextIsolation: true
    }
  });

  const devUrl = 'http://127.0.0.1:5173';
  mainWindow.loadURL(devUrl);
  mainWindow.once('ready-to-show', () => mainWindow.show());

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

function createTray() {
  const iconPath = path.join(process.cwd(), 'public', 'favicon.svg');
  const icon = nativeImage.createFromPath(iconPath);
  tray = new Tray(icon);
  const contextMenu = Menu.buildFromTemplate([
    { label: 'Open Vortex', click: () => mainWindow && mainWindow.show() },
    { label: 'Exit', click: () => app.quit() }
  ]);
  tray.setToolTip('Vortex Anti-Cheat Monitor');
  tray.setContextMenu(contextMenu);
}

app.whenReady().then(() => {
  createWindow();
  createTray();
  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});
