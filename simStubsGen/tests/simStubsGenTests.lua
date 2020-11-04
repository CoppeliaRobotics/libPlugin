function main()
    local struct1={i=10,f=6.6,d=4.444444,s='xx',b=false}

    numPassed,numFailed=0,0

    logInfo('%d/%d tests passed successfully',numPassed,numPassed+numFailed)
end

function log(level,fmt,...)
    sim.addLog(level,string.format('StubsGenTests: '..fmt,...))
end

function logError(fmt,...)
    log(sim.verbosity_errors,fmt,...)
end

function logWarn(fmt,...)
    log(sim.verbosity_warnings,fmt,...)
end

function logInfo(fmt,...)
    log(sim.verbosity_infos,fmt,...)
end

function logDebug(fmt,...)
    log(sim.verbosity_debug,fmt,...)
end

logInfo('simStubsGenTests.lua loaded')

function table.pack(...)
    return {n=select("#",...),...}
end

function table.tostring(t)
    local vals={}
    local maxIntKey=nil
    for i,v in ipairs(t) do
        if type(v)=='table' then v=table.tostring(v) end
        table.insert(vals,tostring(v))
        maxIntKey=i
    end
    for k,v in pairs(t) do
        if (type(k)=='number' and (not maxIntKey or k<1 or k>maxIntKey)) or type(k)~='number' then
            if type(v)=='table' then v=table.tostring(v) end
            table.insert(vals,k..'='..tostring(v))
        end
    end
    local s,sep='{',''
    for _,v in ipairs(vals) do
        s=s..sep..v
        sep=','
    end
    return s..'}'
end

function argsStr(...)
    local s,sep='',''
    local args=table.pack(...)
    for i=1,args.n do
        local v=args[i]
        local sv=type(v)=='table' and table.tostring(v) or tostring(v)
        s=s..sep..sv
        sep=','
    end
    return s
end

function assertEq(a,b)
    if type(a)=='number' and type('b')=='number' then
        return math.abs(a-b)<1e-9
    end
    return a==b
end

function assertOk(n,f)
    logDebug('test "%s"...',n)
    local r,e=pcall(f)
    if not r then
        logDebug('test "%s": error: %s',n,e)
        logError('test "%s": FAILED',n)
        numFailed=numFailed+1
    else
        logInfo('test "%s": OK',n)
        numPassed=numPassed+1
    end
end

function assertFail(n,f)
    logDebug('test "%s"...',n)
    local r,e=pcall(f)
    if r then
        logDebug('test "%s": succeeded, but should have failed',n)
        logError('test "%s": FAILED',n)
        numFailed=numFailed+1
    else
        logDebug('test "%s": error: %s',n,e)
        logInfo('test "%s": OK',n)
        numPassed=numPassed+1
    end
end

function assertEq(a,b)
    assert(type(a)==type(b),'type mismatch')
    if type(a)=='number' then
        assert(math.abs(a-b)<1e-6,'not equal: '..a..' '..b)
    elseif type(a)=='table' then
        for k,v in pairs(a) do
            assertEq(a[k],b[k])
        end
        for k,v in pairs(b) do
            assertEq(a[k],b[k])
        end
    else
        assert(a==b,'not equal: '..tostring(a)..' '..tostring(b))
    end
end

function loadModule()
    local p=sim.getStringNamedParam('simStubsGenTests.module')
    local h=sim.loadModule(p,'StubsGenTests')
    logInfo('loaded module %s with handle %d',p,h)
    assert(h~=-1,'failed loading StubsGenTests module')
    local funcs={}
    for k,v in pairs(simStubsGenTests) do if type(v)=='function' then table.insert(funcs,k) end end
    logInfo('module functions: %s',table.concat(funcs,', '))
end

function callFunc(fname,...)
    logInfo('calling %s(%s)',fname,argsStr(...))
    local f=simStubsGenTests[fname]
    return f(...)
end

function sysCall_init()
    local v=sim.getStringNamedParam('simStubsGenTests.verbosity')
    if v then sim.setInt32Parameter(sim.intparam_verbosity,sim['verbosity_'..v]) end

    logInfo('add-on initialized')
    local r,e=pcall(loadModule)
    if not r then
        logError('aborted: %s',e)
        sim.quitSimulator()
    end
end

function sysCall_nonSimulation()
    local r,e=pcall(main)
    if not r then
        logError('aborted: %s',e)
        sim.quitSimulator()
    end
    sim.quitSimulator()
end

function sysCall_cleanup()
    sim.setInt32Parameter(sim.intparam_verbosity,sim.verbosity_warnings)
end
