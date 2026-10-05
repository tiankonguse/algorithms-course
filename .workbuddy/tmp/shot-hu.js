const file='file:///Users/tiankonguse-m3/project/github/AIProject/algorithms-course/outputs/humanizer-digital-minimalism.html';
const fs=require('fs');
(async()=>{
const list=await(await fetch('http://127.0.0.1:9223/json/list')).json();
const ws=list.find(t=>t.type==='page').webSocketDebuggerUrl;
const sock=new WebSocket(ws);let id=0;const pend=new Map();
const send=(m,p={},s)=>new Promise(r=>{const i=++id;pend.set(i,r);sock.send(JSON.stringify(s?{id:i,method:m,params:p,sessionId:s}:{id:i,method:m,params:p}));});
await new Promise(r=>sock.addEventListener('open',r));
sock.addEventListener('message',ev=>{const m=JSON.parse(ev.data);if(m.id&&pend.has(m.id)){pend.get(m.id)(m);pend.delete(m.id);}});
const t=await send('Target.createTarget',{url:'about:blank'});
const a=await send('Target.attachToTarget',{targetId:t.result.targetId,flatten:true});const sid=a.result.sessionId;
await send('Page.enable',{},sid);
await send('Emulation.setDeviceMetricsOverride',{width:1100,height:1400,deviceScaleFactor:1,mobile:false},sid);
await send('Page.navigate',{url:file},sid);
await new Promise(r=>setTimeout(r,2000));
let r=await send('Page.captureScreenshot',{format:'png'},sid);
fs.writeFileSync('/tmp/hu-top.png',Buffer.from(r.result.data,'base64'));
for(const [i,y] of [[1,1400],[2,2800],[3,4200]].entries()){
  const rr=await send('Page.captureScreenshot',{format:'png',clip:{x:0,y:y,width:1100,height:1400,scale:1}},sid);
  fs.writeFileSync('/tmp/hu-'+y+'.png',Buffer.from(rr.result.data,'base64'));
}
console.log('shot ok');process.exit(0);})();
