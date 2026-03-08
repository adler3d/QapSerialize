// check-missing-frames.js
const fs=require('fs');
const path=require('path');
const readline=require('readline');

const PREFIX='out';
const START_NUM=465087;
const EXT='.png';
const MAX_GAP=20;

function parseIndex(name){
  if(!name.startsWith(PREFIX)||!name.endsWith(EXT))return null;
  const numStr=name.slice(PREFIX.length,name.length-EXT.length);
  if(!/^\d+$/.test(numStr))return null;
  return parseInt(numStr,10);
}

function formatName(idx){
  return PREFIX+idx.toString().padStart(6,'0')+EXT;
}

function getPngFiles(dir){
  return fs.readdirSync(dir)
    .filter(f=>f.endsWith(EXT)&&parseIndex(f)!==null)
    .sort();
}

function findMissing(files){
  const indices=files
    .map(parseIndex)
    .filter(i=>i!==null&&i>=START_NUM)
    .sort((a,b)=>a-b);
  if(indices.length===0)return {missing:[],indices:[]};

  const missing=[];
  for(let i=0;i<indices.length-1;i++){
    const cur=indices[i];
    const next=indices[i+1];
    const gap=next-cur-1;
    if(gap>0&&gap<=MAX_GAP){
      for(let k=cur+1;k<next;k++){
        missing.push(k);
      }
    }
  }
  return {missing,indices};
}

function askYesNo(question){
  return new Promise(resolve=>{
    const rl=readline.createInterface({input:process.stdin,output:process.stdout});
    rl.question(question+' (y/N): ',answer=>{
      rl.close();
      const a=answer.trim().toLowerCase();
      resolve(a==='y'||a==='yes');
    });
  });
}

async function main(){
  const dir=process.cwd();
  const files=getPngFiles(dir);
  if(files.length===0){
    console.log('No PNG files found.');
    return;
  }

  const {missing,indices}=findMissing(files);
  if(missing.length===0){
    console.log('No missing frames detected (within max gap of '+MAX_GAP+').');
    return;
  }

  console.log('Missing frames (within sequences with gap <= '+MAX_GAP+'):');
  const missingNames=missing.map(formatName);
  missingNames.forEach(n=>console.log('  '+n));

  const confirm=await askYesNo('Copy previous frame for ALL missing files?');
  if(!confirm){
    console.log('No files created.');
    return;
  }

  // Build a set of existing indices for quick lookup
  const existingSet=new Set(indices);

  for(const idx of missing){
    const target=path.join(dir,formatName(idx));

    // find previous existing index
    let prevIdx=idx-1;
    while(prevIdx>=START_NUM&&!existingSet.has(prevIdx)){
      prevIdx--;
    }
    if(prevIdx<START_NUM){
      console.log('Skipping '+target+' (no previous existing frame found).');
      continue;
    }

    const src=path.join(dir,formatName(prevIdx));
    try{
      fs.copyFileSync(src,target);
      console.log('Created '+target+' from '+path.basename(src));
      existingSet.add(idx);
    }catch(e){
      console.error('Failed to create '+target+':',e.message);
    }
  }
}

main().catch(e=>{
  console.error('Error:',e);
  process.exit(1);
});
