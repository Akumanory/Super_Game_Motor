print('scr')
json = require 'dkjson'
debuggee = require 'vscode-debuggee'
startResult, breakerType = debuggee.start(json)
print('debuggee start ->', startResult, breakerType)

dofact = require 'fact'
dofact()