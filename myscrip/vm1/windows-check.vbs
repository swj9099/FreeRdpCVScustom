Option Explicit

'shellΪ�ű�����scriptsDictΪ�ֵ����adodbStreamΪ�ļ���д����ScriptHostNameΪ�ű�����·����ScriptNameΪ�ű����ƣ�localIPΪ����ip
'uuidTextΪģ��id��resultXMLnameΪ����ļ�����ScriptPathΪvbs�ű�����·��,localLangΪ�����ַ�����ʶ
'initScriptResultΪ��ʼ�������ű�ִ�н����regexΪ�������,scriptIdArray()Ϊ��Žű�id������

Dim shell,scriptsDict,adodbStream,scriptHostName,localIP,uuidText,localLang,scriptArgs
Dim processEnvs, scriptXMLFile,resultXMLname,scriptPath,initScriptResult,regex,scriptIdArray()
Set shell = createobject("Wscript.Shell")
Set regex=new RegExp
Set adodbStream = CreateObject("ADODB.Stream")
Set scriptsDict = CreateObject("Scripting.Dictionary")
Set scriptArgs = WScript.Arguments

scriptHostName = WScript.FullName
scriptPath = replace(Wscript.ScriptFullName,Wscript.ScriptName,"")

If LCase(right(scriptHostName, len(scriptHostName)-InStrRev(scriptHostName,"\"))) = "wscript.exe" Then
    Dim cmdStr
	cmdStr = "cmd /c title �����Թ���ϵͳ-���ú˲����߹��� &&  cscript.exe //nologo " & chr(34) & wscript.scriptfullname & chr(34) 
    shell.run cmdStr
    Wscript.quit
end if

WScript.Echo ""
WScript.Echo "**************************************************************************" 
WScript.Echo "*                                                                        *"
WScript.Echo "*                    �����Թ���ϵͳ-���ú˲����߹���                     *"  
WScript.Echo "*                            CVS V6.80.3                                 *"                     
WScript.Echo "*                            2017/10/12                                  *"
WScript.Echo "*                                                                        *"
WScript.Echo "**************************************************************************"
WScript.Echo "* ��Ȩ����:                                                              *"
WScript.Echo "* ������ṩ��������������Ͼ�Ϊ��Ȩ****����,δ��xxxx������ɣ��κε�λ��*"
WScript.Echo "* ���˲������κη�ʽ���ơ�ת�ء����ӡ�ת�������á�����                   *"
WScript.Echo "*                                                                        *"
WScript.Echo "* ��������:                                                              *"
WScript.Echo "* �����ں˲�ϵͳ��ȫ��������,��������ط��ɷ���,�����ڷǷ���;           *"
WScript.Echo "**************************************************************************"
WScript.Echo ""

'���Դ���,��ɨ�跢������ʱ���������Դ����õģ�������ܷ������󣬾���֮ǰ�ĵط�����
'On Error Resume Next
'call detectError()

'���ش�ִ�е�ɨ��ű��ļ�
scriptXMLFile = "scripts.dat"
Set processEnvs=shell.Environment("Process")

'��ȡ����IP���ַ�����Ϣ
call checkArguments()

WScript.Echo "�˲鿪ʼ ..."

'�Ѳ������뵽���̵Ļ�������
call setArguments2Envs()

'���ؽű�XML
call loadScriptsFromXML()

'���ɽ��xml��ǰ�벿��
call doPreoADoWrite()

'ִ�нű�ɨ�貢׷�ӵ�XML�����
call checkWindows()

'���ɽ��xml�ĺ�벿��
call doEndoADoWrite()

WScript.Echo ""
WScript.Echo "�˲���������:" & resultXMLname
WScript.Echo ""
WScript.Echo "Windows�����˲����,5����Զ��˳�..."
WScript.Sleep 5000

'�������õ�����������
Function setArguments2Envs()
  
				
End Function

Function  checkArguments()
  Dim NIC1,Nic,StrIP,langResult
  If scriptArgs.Count > 0 then
	
	WScript.Echo "Usage:cscript "&WScript.ScriptName&""
    WScript.Echo "      host_ip: host 's ip address"
    WScript.Echo "      Example: cscript "&WScript.ScriptName&""
				
	Wscript.quit
  Elseif scriptArgs.Count = 1 then
    localIP = scriptArgs(0)
  Else
    '��δ����IP��ַ����ʱ�Զ�ʶ�𱾻���ЧIP
	Set NIC1 = getObject("winmgmts:").InstancesOf("Win32_NetworkAdapterConfiguration")
	For Each Nic in NIC1
	    If Nic.IPEnabled then
	      StrIP = Nic.IPAddress(0)
	        If Nic.ServiceName <> "VMnetAdapter" and StrIP <> "0.0.0.0" and InStr(StrIP, "169.254") = 0 then
	         localIP = StrIP
	        End If
	    End If
	Next
  End if
  'ʶ���ַ�������
  Set langResult = shell.exec("cmd /c chcp")
  langResult.stdin.close 
  If langResult.exitcode = 0 then
	localLang = encodebase64Data(langResult.stdout.readall)
  Else
	localLang = encodebase64Data("commands execute failed")
  End if
End Function



'��ɨ�跢������ʱ���������Դ����õģ�������ܷ������󣬾���֮ǰ�ĵط����� call detectError �����ԡ�
Function detectError()
	If Err.Number<>0 Then  
	  WScript.Echo Err.Number & Err.Description & Err.Source 
	  WScript.Echo "�˲�ʱ��������,5����Զ��˳�..."	  
	  WScript.Sleep 5000
	  WScript.quit
	End If
End Function 


Function  checkWindows()
  Dim cmdIds,cmdList,k,promptString
  promptString= scriptPath & ">"
  cmdIds = scriptsDict.Keys
  cmdList = scriptsDict.Items
  resultXMLname = ScriptPath & localIP & "_" & uuidText & ".xml"
  For k=0 to Ubound(scriptIdArray)
    Dim cmdResult,rawdata
	WScript.Echo "�˲���ȣ�" & k+1 & "/" & scriptsDict.count
	adodbStream.WriteText "		<script>" & VbCrLf
	adodbStream.WriteText "			<id>"&scriptIdArray(k)& "</id>"&VbCrLf
	adodbStream.WriteText "			<value>"
	rawdata = executeScript (scriptIdArray(k), scriptsDict(scriptIdArray(k)) )
	rawdata= rawdata & VbCrLf & promptString
	'WScript.Echo rawdata '���Դ���
	adodbStream.WriteText encodebase64Data(rawdata) & "</value>" & VbCrLf
	adodbStream.WriteText "		</script>" & VbCrLf
  Next
End Function

'�ѽű�д��bat��ִ������Ϊ���������п�������ʱ�������ֿ�ִ��ʱ����ʱ������Ч;
'������������ַ���ִ��ʱ vbs����Ե�һ���Ժ��������;
'����&ƴ�Ӷ������ʱ���ص�ԭʼ��ʽ�������ߵĲ�һ���ˣ���������������������
Function executeScript(scriptId,scripts)
  Dim fileSystemObject,scriptFileName,scriptFile,batScriptResult
  scriptFileName=scriptPath & "vm-scripts-" & scriptId & ".bat"
  Set fileSystemObject = CreateObject("Scripting.FileSystemObject")
  Set scriptFile = fileSystemObject.CreateTextFile(scriptFileName)
  Dim regexMatchObj,reMatchitem,batScript
  regex.Pattern="^\s*(\S.*\S)\s*$"
  regex.Global=True
  regex.MultiLine=True
  set regexMatchObj=regex.execute(scripts)
  If regexMatchObj.Count <> 0 then
	  For each reMatchitem in regexMatchObj
		batScript=regex.Replace(reMatchitem,"$1")
		scriptFile.writeline(batScript)
	  Next
  End if
  Set batScript=shell.exec("cmd /c " & scriptFileName)
  batScript.stdin.close
  batScriptResult=batScript.stdout.readall
  scriptFile.close
  'WScript.Sleep 2000 '���Դ���
  fileSystemObject.DeleteFile(scriptFileName)
  executeScript=batScriptResult
End Function

Function loadScriptsFromXML()
  Dim scriptsXML,xmlRoot,uuidObj,envObj,envCmds,scriptArr
  Set scriptsXML=createobject("Microsoft.XMLDOM")
  scriptsXML.async=false
  shell.run "cmd /c del /Q ""%USERPROFILE%\" & scriptXMLFile & """",0,true
  shell.run "cmd /c copy """ & ScriptPath & scriptXMLFile & """  """ & shell.ExpandEnvironmentStrings("%USERPROFILE%") & """",0,true
  scriptsXML.load(shell.ExpandEnvironmentStrings("%USERPROFILE%") & "\" & scriptXMLFile)
  'scriptsXML.load(ScriptPath & scriptXMLFile)
  If scriptsXML.parseError.errorCode <> 0 Then 
	Wscript.Echo "����:" & scriptsXML.parseError.reason 
  End If 
  Set xmlRoot=scriptsXML.documentElement
  Set uuidObj=xmlRoot.firstchild
  uuidText=uuidObj.childnodes(0).nodevalue
  Set envObj=xmlRoot.selectsinglenode("env")
  envCmds=trimScript(envObj.childnodes(0).nodevalue)
  Set scriptArr=scriptsXML.getelementsbytagname("script")
  Dim i 
  For i=0 to scriptArr.length-1
    Dim scriptId,scriptCmd
    scriptId=scriptArr(i).firstchild.childnodes(0).nodevalue
    scriptCmd=scriptArr(i).selectsinglenode("cmd").childnodes(0).nodevalue
    scriptCmd=trimScript(scriptCmd)
    scriptsDict.Add scriptId,scriptCmd
    ReDim Preserve scriptIdArray(scriptArr.length-1)
    scriptIdArray(i)=scriptId
  Next
  call setScriptEnv(envCmds)
  shell.run "cmd /c del /Q ""%USERPROFILE%\" & scriptXMLFile & """",0,true
  If err.number<>0 then
	Wscript.Echo "Error:" &  Err.Number & Err.Description
	Wscript.quit
  End If
End Function
 
Function setScriptEnv(envCmds)
  Dim fileSystemObject,batScriptFile,setCmdResult,cmdItem,batScriptResult
  Set fileSystemObject = CreateObject("Scripting.FileSystemObject")
  Dim preEnvCmdBatFile
  preEnvCmdBatFile="initPreEnvScripts.bat"
  Set batScriptFile = fileSystemObject.CreateTextFile(scriptPath & preEnvCmdBatFile)
  Dim regexMatchObj,reMatchitem,batScript 
  regex.Pattern="^\s*(\S.*\S)\s*$" 
  regex.Global=True 
  regex.MultiLine=True 
  set regexMatchObj=regex.execute(envCmds) 
  If regexMatchObj.Count <> 0 then
	For each reMatchitem in regexMatchObj
		batScript=regex.Replace(reMatchitem,"$1")
		batScriptFile.writeline(batScript)
	Next
  End if
  Set batScript=shell.exec("cmd /c "&scriptPath & preEnvCmdBatFile)
  batScript.stdin.close
  batScriptResult=batScript.stdout.readall
  batScriptFile.close
  fileSystemObject.DeleteFile(scriptPath & preEnvCmdBatFile)

  setCmdResult=getSetCmdResult(batScriptResult)
  Dim regexMatch,matchItem
  regex.Pattern="([\w|_]+?)=(.*)"
  regex.Global=True
  regex.MultiLine=True
  Set regexMatch=regex.execute(setCmdResult)
  If regexMatch.Count <> 0 then
    For each matchItem in regexMatch
      Dim varName,varValue
      varName=regex.Replace(matchItem.value,"$1")
      varValue=regex.Replace(matchItem.value,"$2")
	  'WScript.Echo varName & "=" & varValue '���Դ���
      processEnvs(varName)=varValue
    Next 
  End if
End Function


Function getSetCmdResult(resultString)
  Dim regexMatch,envUuidStr
  envUuidStr="VM-DETECT-ENV-FLAG"
  regex.Pattern="[\s\S]*?" & envUuidStr & "\s*(\S[\s\S]+\S)\s*"
  regex.Global=True
  Set regexMatch=regex.execute(resultString)
  If regexMatch.Count <> 0 then
    getSetCmdResult=regex.Replace(resultString,"$1")
  End if
End Function

Function trimScript(ScriptString)
  Dim regexMatch
  regex.Pattern="\s*(\S[\s\S]+\S)\s*"
  regex.Global=True
  Set regexMatch=regex.execute(ScriptString)
    If regexMatch.Count <> 0 then
      trimScript=regex.Replace(ScriptString,"$1")
    End if
End Function

Function getHostName()
	Dim objNTInfo,ComputerName
	Set objNTInfo = CreateObject("WinNTSystemInfo")
	ComputerName = objNTInfo.ComputerName
	GetHostName = ComputerName
End Function

Sub doPreoADoWrite()
	Dim startTime
	startTime = year(Now)&"-"&Month(Now)&"-"&day(Now) & " " & Hour(Now)&":"&Minute(Now)&":"&Second(Now)
	adodbStream.Charset = "utf-8"
	adodbStream.Open
	adodbStream.WriteText "<?xml version=""1.0"" encoding=""utf-8""?>" &VbCrLf
	adodbStream.WriteText "<result>" &VbCrLf
	adodbStream.WriteText "	<ip>"&localIP&"</ip>" &VbCrLf
	adodbStream.WriteText "	<hostname>"&GetHostName()&"</hostname>" &VbCrLf
	adodbStream.WriteText "	<uuid>"&uuidText&"</uuid>" &VbCrLf
	adodbStream.WriteText "	<lang>"& localLang &"</lang>"&VbCrLf
	adodbStream.WriteText "	<start-time>"&startTime&"</start-time>" &VbCrLf
	adodbStream.WriteText "	<scripts>"&VbCrLf
End Sub

Sub doEndoADoWrite()
    Dim endTime
	adodbStream.Writetext "	</scripts>" &VbCrLf
	endTime = year(Now)&"-"&Month(Now)&"-"&day(Now) & " " & Hour(Now)&":"&Minute(Now)&":"&Second(Now)
	adodbStream.WriteText "	<end-time>"&endTime&"</end-time>"&VbCrLf
	adodbStream.WriteText "</result>"
	adodbStream.SaveToFile resultXMLname, 2
	adodbStream.close
	Set adodbStream = nothing
End Sub

'�����ݽ���base64����
Function encodebase64Data(rawdata)
    Dim objXmlDom,objNode,data,newdata,tmp,m,res,num,j,i
	Set objXmlDom = CreateObject("Microsoft.XMLDOM")  
	Set objNode = objXmlDom.CreateElement("Binary")  
	objNode.DataType = "bin.hex"  
	With CreateObject("ADODB.Stream")  
		.Type = 2:.Charset = "UTF-8":.Open  
		.WriteText rawdata  
		.Position = 0:.Type = 1  
		objNode.nodeTypedValue = .Read  
		.Close  
	End With  
	data = Mid(objNode.Text,7)  
	newdata = ""  
    For j = 1 To Len(data) Step 2  
        tmp = CLng("&H"&Mid(data,j,2))  
        m = ""  
        Do  
            m = m & tmp mod 2  
            tmp = tmp\2  
        Loop Until tmp=0  
        m = StrReverse(m)  
        Do While Len(m)<8  
            m = "0" & m  
        Loop  
        newdata = newdata & m  
    Next  
    res = ""  
    For j = 1 To Len(newdata) Step 6  
        tmp = Mid(newdata, j, 6)  
        num = 0  
        For i = 1 To 6  
            m = Mid(tmp, i, 1)  
            If m = "" Then m = 0  
            num = num * 2 + m  
        Next  
        If num <= 25 Then  
            res = res & chr(65+num)  
        ElseIf num <= 51 Then  
            res = res & chr(71+num)  
        ElseIf num <=61 Then  
            res = res & chr(num-4)  
        ElseIf num = 62 Then  
            res = res & "+"  
        ElseIf num = 63 Then  
            res = res & "/"  
        End If  
    Next  
    tmp = Len(res) mod 4  
    If tmp <>0 Then  
        For j = 1 To 4-tmp  
            res = res & "="  
        Next  
    End If  
    encodebase64Data =  res
End Function