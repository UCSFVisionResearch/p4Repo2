Option Strict Off
Option Explicit On
Imports Microsoft.VisualBasic.PowerPacks
Friend Class frmTDTComputer
	Inherits System.Windows.Forms.Form
	Public Last As Single
	Public iListSize, RPconnected As Short
	Public Running As Boolean
	Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Integer)
	
	Private Sub BLocalPort_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles BLocalPort.Click
		udpTDTComputer.Close()
		udpTDTComputer.Bind(Val(BLocalPort.Text))
	End Sub
	Private Sub Quit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Quit.Click
		Dim f As System.Windows.Forms.Form
		For	Each f In My.Application.OpenForms
			f.Close()
		Next f
		End
	End Sub
	Private Sub StartSend_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles StartSend.Click
		If RPconnected <> 1 Then
			If RPcoX1.ConnectRZ5("GB", 1) Then
				'       MsgBox "Connection established"
				RPconnected = 1
			Else
				MsgBox("Error connecting to RZ5")
			End If
		End If
	End Sub
	Private Sub StopSend_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles StopSend.Click
		Running = False
	End Sub
	Private Sub frmTDTComputer_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		Dim a As Object
		'Setup connection to TDTDevAcc
		'UPGRADE_WARNING: Couldn't resolve default property of object a. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        TDevAccX1.ConnectServer("Local")
		If TDevAccX1.CheckServerConnection = 0 Then
			MsgBox("TDevAccX1.CheckServerConnection: Open your TDT project and  Workbench is in Record or Preview Mode before restarting this VB program")
			End
		Else
			'    MsgBox ("TDevAccX1.CheckServerConnection succeded")
			' The control's name is udpTDTComputer.
            udpTDTComputer.Bind(Val(BLocalPort.Text), ) ' Bind to the default local port.
			iListSize = 0
		End If
		
		TrigShape.Visible = True
		TrigShape.FillColor = System.Drawing.ColorTranslator.FromOle(&HFF0000) 'blue
		RPconnected = 0
		
		If RPconnected = 0 Then
			RPconnected = RPcoX1.ConnectRZ5("GB", 1)
		End If
		If RPconnected <> 1 Then
			MsgBox("Error connecting to RZ5")
		End If
		
	End Sub
	
	Private Sub udpTDTComputer_DataArrival(ByVal eventSender As System.Object, ByVal eventArgs As AxMSWinsockLib.DMSWinsockControlEvents_DataArrivalEvent) Handles udpTDTComputer.DataArrival
		Dim EZ7 As Object
		Dim EZ5 As Object
		Dim EZ4 As Object
		Dim EZ6 As Object
		Dim EZ2 As Object
		Dim EZ1 As Object
		Dim EZ3 As Object
		Dim c_ind As Object
		Dim b_ind As Object
		Dim a_ind As Object
		Dim c_val As Object
		Dim b_val As Object
		Dim a_val As Object
		Dim cond As Object
		Dim c As Object
		Dim strData As String
		
		If RPconnected = 1 Then 'Get the data only if the input is connected, RPvds file must be in Run mode
			TrigShape.FillColor = System.Drawing.ColorTranslator.FromOle(&HFF00) 'green
			TrigShape.Visible = True
			TrigShape.Refresh()
			
			
			udpTDTComputer.GetData(strData)
			txtOutput.Text = strData
			lstOutput.Items.Add(strData)
			
			'Write values and indices for next epoc
			
			If (strData <> "A") Then
				' do if not movie (sends "A")
				'UPGRADE_WARNING: Couldn't resolve default property of object c. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				c = Split(strData, " ", -1)
				'UPGRADE_WARNING: Couldn't resolve default property of object cond. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				cond = Int(CDbl(c(0)))
				'UPGRADE_WARNING: Couldn't resolve default property of object c(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object a_val. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				a_val = Val(c(1))
				'UPGRADE_WARNING: Couldn't resolve default property of object c(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object b_val. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				b_val = Val(c(2))
				'UPGRADE_WARNING: Couldn't resolve default property of object c(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object c_val. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				c_val = Val(c(3))
				'UPGRADE_WARNING: Couldn't resolve default property of object a_ind. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				a_ind = Int(CDbl(c(4)))
				'UPGRADE_WARNING: Couldn't resolve default property of object b_ind. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				b_ind = Int(CDbl(c(5)))
				'UPGRADE_WARNING: Couldn't resolve default property of object c_ind. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				c_ind = Int(CDbl(c(6)))
				'UPGRADE_WARNING: Couldn't resolve default property of object cond. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object EZ3. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				EZ3 = TDevAccX1.SetTargetVal("Amp1.CondNum", cond)
				'UPGRADE_WARNING: Couldn't resolve default property of object a_val. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object EZ1. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				EZ1 = TDevAccX1.SetTargetVal("Amp1.A_value", a_val)
				'UPGRADE_WARNING: Couldn't resolve default property of object b_val. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object EZ2. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				EZ2 = TDevAccX1.SetTargetVal("Amp1.B_value", b_val)
				'UPGRADE_WARNING: Couldn't resolve default property of object c_val. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object EZ6. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				EZ6 = TDevAccX1.SetTargetVal("Amp1.C_value", c_val)
				'UPGRADE_WARNING: Couldn't resolve default property of object a_ind. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object EZ4. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				EZ4 = TDevAccX1.SetTargetVal("Amp1.A_index", a_ind)
				'UPGRADE_WARNING: Couldn't resolve default property of object b_ind. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object EZ5. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				EZ5 = TDevAccX1.SetTargetVal("Amp1.B_index", b_ind)
				'UPGRADE_WARNING: Couldn't resolve default property of object c_ind. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object EZ7. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				EZ7 = TDevAccX1.SetTargetVal("Amp1.C_index", c_ind)
				
			End If
			
			
			
			TrigShape.Visible = False
			TrigShape.Refresh()
			
			'we will probabaly stop this if no need for debugging
			iListSize = iListSize + 1
			If iListSize > 25 Then lstOutput.Items.RemoveAt(0)
		Else
			MsgBox("Error connecting to RZ5")
		End If 'RPconnected
	End Sub
End Class