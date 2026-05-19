import { contextBridge } from 'electron';

contextBridge.exposeInMainWorld('vortexApi', {
  ping: () => 'vortex-ready'
});
