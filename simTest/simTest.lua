function os.capture(cmd,raw)
    local f=assert(io.popen(cmd,'r'))
    local s=assert(f:read('*a'))
    f:close()
    if raw then return s end
    s=string.gsub(s,'^%s+','')
    s=string.gsub(s,'%s+$','')
    s=string.gsub(s,'[\n\r]+',' ')
    return s
end

function os.platform()
    local win=package.config:sub(1,1)=='\\'
    if win then return 'windows' end
    local uname=os.capture('uname')
    if uname=='Linux' then return 'linux' end
    if uname=='Darwin' then return 'macos' end
    return 'unknown'
end

simTest={}

function simTest.log(v,msg)
    sim.addLog(v,'simTest: '..msg)
end

function simTest.logInfo(msg)
    simTest.log(sim.verbosity_errors,msg)
end

function simTest.logWarning(msg)
    simTest.log(sim.verbosity_errors,msg)
end

function simTest.logError(msg)
    simTest.log(sim.verbosity_errors,msg)
end

function simTest.fatalError(msg)
    simTest.logError(msg)
    sim.quitSimulator(true)
end

function simTest.listLuaFiles(dirName)
    local files={}
    local plat=os.platform()
    local g
    if plat=='windows' then
        g=io.popen('dir "'..dirName..'" /b /ad')
    else
        g=io.popen('ls -1 "'..dirName..'"')
    end
    for file in g:lines() do
        if file:match('%.lua$') then
            table.insert(files,file)
        end
    end
    return files
end

function simTest.getFileContent(fileName)
    local f=io.open(fileName,'rb')
    if not f then
        simTest.fatalError('file "'..luaFile..'" does not exist')
    end
    local content=f:read('*a')
    f:close()
    return content
end

function simTest.getMeta(luaCode)
    local meta={}
    for k, v in string.gmatch(luaCode,'\n--@(%w+) ([^\n]*)\n') do
        meta[k]=v
    end
    return meta
end

function simTest.createDummyWithScript(objName,luaCode,scriptType)
    local dummyHandle=sim.createDummy(0.01)
    sim.setObjectName(dummyHandle,objName)
    local scriptHandle=sim.addScript(scriptType)
    sim.associateScriptWithObject(scriptHandle,dummyHandle)
    sim.setScriptText(scriptHandle,luaCode)
    return dummyHandle
end

function simTest.writeOutputFile(fileName,content)
    local output_dir=sim.getStringNamedParam('output_dir')
    f=io.open(output_dir..'/'..fileName,'w+')
    f:write(content)
    f:close()
end

function simTest.setTestResult(r)
    simTest.writeOutputFile('result.txt',r)
end

function simTest.setTestExitCode(e)
    simTest.writeOutputFile('exitcode.txt',e)
end

function simTest.success(msg)
    if msg==nil then
        simTest.logInfo('test succeeded')
    else
        simTest.logInfo('test succeeded: '..msg)
    end
    simTest.setTestResult('success')
    simTest.setTestExitCode(0)
    sim.quitSimulator(true)
end

function simTest.failure(msg)
    if msg==nil then
        simTest.logInfo('test failed')
    else
        simTest.logInfo('test failed: '..msg)
    end
    simTest.setTestResult('failure')
    simTest.setTestExitCode(1)
    sim.quitSimulator(true)
end

function sysCall_init()
    simTest.logInfo('started (platform='..os.platform()..')')

    -- add simTest's dir to Lua's package.path:
    local simTest_dir=sim.getStringNamedParam('simTest_dir')
    local dirSep=package.config:sub(1,1)
    local pathSep=package.config:sub(3,3)
    local luaWildCard=package.config:sub(5,5)
    package.path=string.format('%s%s%s.lua%s%s',simTest_dir,dirSep,luaWildCard,pathSep,package.path)
    sim.registerScriptVariable('package.path','"'..package.path..'"')
    sim.executeScriptString('package.path="'..package.path..'"@',sim.scripttype_sandboxscript)

    -- read .lua files from input_dir:
    local input_dir=sim.getStringNamedParam('input_dir')
    local luaFiles=simTest.listLuaFiles(input_dir)
    haveChildScripts=false
    haveCustomizationScripts=false
    local initCode={}
    for i,luaFile in ipairs(luaFiles) do
        simTest.logInfo('processing lua file "'..luaFile..'"')
        local luaCode=simTest.getFileContent(input_dir..'/'..luaFile)
        local meta=simTest.getMeta(luaCode)
        if meta.mode==nil then
            simTest.fatalError('script "'..luaFile..'" does not contain a "mode" parameter')
        end
        local threaded=meta.threaded=='true' or meta.threaded=='1'
        if meta.mode=='init' then
            initCode[luaFile]=luaCode
        elseif meta.mode=='child' then
            simTest.logInfo('added child script for '..luaFile)
            simTest.createDummyWithScript('simTest_'..luaFile:gsub('%.lua$',''),luaCode,sim.scripttype_childscript+(threaded and sim.scripttype_threaded or 0))
            haveChildScripts=true
        elseif meta.mode=='customization' then
            simTest.logInfo('added customization script for '..luaFile)
            simTest.createDummyWithScript('simTest_'..luaFile:gsub('%.lua$',''),luaCode,sim.scripttype_customizationscript)
            simTest.logInfo('haveCustomizationScripts=true')
            haveCustomizationScripts=true
        else
            simTest.fatalError('invalid value "'..meta.mode..'" for parameter "mode" in script "'..luaFile..'"')
        end
    end

    -- execute scripts with '@mode init'
    for luaFile,code in pairs(initCode) do
        simTest.logInfo('executing init code for '..luaFile)
        result,value=sim.executeScriptString(code..'@',sim.scripttype_sandboxscript)
        if result==-1 then
            simTest.fatalError('error in '..luaFile..': '..value)
        end
    end

    if haveCustomizationScripts then
        simTest.logInfo('skipping control of the simulation because there are customization scripts')
    else
        if haveChildScripts then
            simTest.logInfo('auto-starting the simulation...')
            sim.startSimulation()
        else
            simTest.logInfo('quitting because there are no child/customization scripts to run')
            sim.quitSimulator(true)
        end
    end
end

function sysCall_afterSimulation()
    --if not haveCustomizationScripts then
    if haveCustomizationScripts==false then
        simTest.logInfo('simulation has stopped. quitting...')
        sim.quitSimulator(true)
    end
end

return simTest
