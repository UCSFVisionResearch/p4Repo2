<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmTDTComputer
#Region "Windows Form Designer generated code "
	<System.Diagnostics.DebuggerNonUserCode()> Public Sub New()
		MyBase.New()
		'This call is required by the Windows Form Designer.
		InitializeComponent()
	End Sub
	'Form overrides dispose to clean up the component list.
	<System.Diagnostics.DebuggerNonUserCode()> Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
		If Disposing Then
			If Not components Is Nothing Then
				components.Dispose()
			End If
		End If
		MyBase.Dispose(Disposing)
	End Sub
	'Required by the Windows Form Designer
	Private components As System.ComponentModel.IContainer
	Public ToolTip1 As System.Windows.Forms.ToolTip
	Public WithEvents Quit As System.Windows.Forms.Button
	Public WithEvents StopSend As System.Windows.Forms.Button
	Public WithEvents StartSend As System.Windows.Forms.Button
	Public WithEvents RPcoX1 As AxRPCOXLib.AxRPcoX
	Public WithEvents TDevAccX1 As AxTDEVACCXLib.AxTDevAccX
	Public WithEvents lstOutput As System.Windows.Forms.ListBox
	Public WithEvents BLocalPort As System.Windows.Forms.TextBox
	Public WithEvents txtOutput As System.Windows.Forms.TextBox
	Public WithEvents udpTDTComputer As AxMSWinsockLib.AxWinsock
	Public WithEvents Label4 As System.Windows.Forms.Label
	Public WithEvents TrigShape As Microsoft.VisualBasic.PowerPacks.RectangleShape
	Public WithEvents Label3 As System.Windows.Forms.Label
	Public WithEvents Label2 As System.Windows.Forms.Label
	Public WithEvents Label1 As System.Windows.Forms.Label
	Public WithEvents ShapeContainer1 As Microsoft.VisualBasic.PowerPacks.ShapeContainer
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmTDTComputer))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.ShapeContainer1 = New Microsoft.VisualBasic.PowerPacks.ShapeContainer
		Me.Quit = New System.Windows.Forms.Button
		Me.StopSend = New System.Windows.Forms.Button
		Me.StartSend = New System.Windows.Forms.Button
		Me.RPcoX1 = New AxRPCOXLib.AxRPcoX
		Me.TDevAccX1 = New AxTDEVACCXLib.AxTDevAccX
		Me.lstOutput = New System.Windows.Forms.ListBox
		Me.BLocalPort = New System.Windows.Forms.TextBox
		Me.txtOutput = New System.Windows.Forms.TextBox
		Me.udpTDTComputer = New AxMSWinsockLib.AxWinsock
		Me.Label4 = New System.Windows.Forms.Label
		Me.TrigShape = New Microsoft.VisualBasic.PowerPacks.RectangleShape
		Me.Label3 = New System.Windows.Forms.Label
		Me.Label2 = New System.Windows.Forms.Label
		Me.Label1 = New System.Windows.Forms.Label
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		CType(Me.RPcoX1, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.TDevAccX1, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.udpTDTComputer, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.Text = "TDT Computer"
		Me.ClientSize = New System.Drawing.Size(381, 510)
		Me.Location = New System.Drawing.Point(4, 30)
		Me.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultLocation
		Me.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.MaximizeBox = True
		Me.MinimizeBox = True
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ShowInTaskbar = True
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "frmTDTComputer"
		Me.Quit.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.Quit.Text = "Quit"
		Me.Quit.Size = New System.Drawing.Size(65, 41)
		Me.Quit.Location = New System.Drawing.Point(200, 104)
		Me.Quit.TabIndex = 9
		Me.Quit.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Quit.BackColor = System.Drawing.SystemColors.Control
		Me.Quit.CausesValidation = True
		Me.Quit.Enabled = True
		Me.Quit.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Quit.Cursor = System.Windows.Forms.Cursors.Default
		Me.Quit.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Quit.TabStop = True
		Me.Quit.Name = "Quit"
		Me.StopSend.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.StopSend.Text = "StopSend"
		Me.StopSend.Size = New System.Drawing.Size(73, 41)
		Me.StopSend.Location = New System.Drawing.Point(112, 104)
		Me.StopSend.TabIndex = 8
		Me.StopSend.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.StopSend.BackColor = System.Drawing.SystemColors.Control
		Me.StopSend.CausesValidation = True
		Me.StopSend.Enabled = True
		Me.StopSend.ForeColor = System.Drawing.SystemColors.ControlText
		Me.StopSend.Cursor = System.Windows.Forms.Cursors.Default
		Me.StopSend.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.StopSend.TabStop = True
		Me.StopSend.Name = "StopSend"
		Me.StartSend.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.StartSend.Text = "StartSend"
		Me.StartSend.Size = New System.Drawing.Size(73, 41)
		Me.StartSend.Location = New System.Drawing.Point(32, 104)
		Me.StartSend.TabIndex = 7
		Me.StartSend.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.StartSend.BackColor = System.Drawing.SystemColors.Control
		Me.StartSend.CausesValidation = True
		Me.StartSend.Enabled = True
		Me.StartSend.ForeColor = System.Drawing.SystemColors.ControlText
		Me.StartSend.Cursor = System.Windows.Forms.Cursors.Default
		Me.StartSend.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.StartSend.TabStop = True
		Me.StartSend.Name = "StartSend"
		RPcoX1.OcxState = CType(resources.GetObject("RPcoX1.OcxState"), System.Windows.Forms.AxHost.State)
		Me.RPcoX1.Location = New System.Drawing.Point(304, 104)
		Me.RPcoX1.Name = "RPcoX1"
		TDevAccX1.OcxState = CType(resources.GetObject("TDevAccX1.OcxState"), System.Windows.Forms.AxHost.State)
		Me.TDevAccX1.Location = New System.Drawing.Point(312, 56)
		Me.TDevAccX1.Name = "TDevAccX1"
		Me.lstOutput.Size = New System.Drawing.Size(257, 345)
		Me.lstOutput.Location = New System.Drawing.Point(16, 152)
		Me.lstOutput.TabIndex = 3
		Me.lstOutput.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.lstOutput.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.lstOutput.BackColor = System.Drawing.SystemColors.Window
		Me.lstOutput.CausesValidation = True
		Me.lstOutput.Enabled = True
		Me.lstOutput.ForeColor = System.Drawing.SystemColors.WindowText
		Me.lstOutput.IntegralHeight = True
		Me.lstOutput.Cursor = System.Windows.Forms.Cursors.Default
		Me.lstOutput.SelectionMode = System.Windows.Forms.SelectionMode.One
		Me.lstOutput.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.lstOutput.Sorted = False
		Me.lstOutput.TabStop = True
		Me.lstOutput.Visible = True
		Me.lstOutput.MultiColumn = False
		Me.lstOutput.Name = "lstOutput"
		Me.BLocalPort.AutoSize = False
		Me.BLocalPort.Size = New System.Drawing.Size(49, 19)
		Me.BLocalPort.Location = New System.Drawing.Point(224, 24)
		Me.BLocalPort.TabIndex = 1
		Me.BLocalPort.Text = "8938"
		Me.BLocalPort.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.BLocalPort.AcceptsReturn = True
		Me.BLocalPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.BLocalPort.BackColor = System.Drawing.SystemColors.Window
		Me.BLocalPort.CausesValidation = True
		Me.BLocalPort.Enabled = True
		Me.BLocalPort.ForeColor = System.Drawing.SystemColors.WindowText
		Me.BLocalPort.HideSelection = True
		Me.BLocalPort.ReadOnly = False
		Me.BLocalPort.Maxlength = 0
		Me.BLocalPort.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.BLocalPort.MultiLine = False
		Me.BLocalPort.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.BLocalPort.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.BLocalPort.TabStop = True
		Me.BLocalPort.Visible = True
		Me.BLocalPort.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.BLocalPort.Name = "BLocalPort"
		Me.txtOutput.AutoSize = False
		Me.txtOutput.Size = New System.Drawing.Size(177, 19)
		Me.txtOutput.Location = New System.Drawing.Point(16, 24)
		Me.txtOutput.TabIndex = 0
		Me.txtOutput.Text = "Text Received"
		Me.txtOutput.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.txtOutput.AcceptsReturn = True
		Me.txtOutput.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtOutput.BackColor = System.Drawing.SystemColors.Window
		Me.txtOutput.CausesValidation = True
		Me.txtOutput.Enabled = True
		Me.txtOutput.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtOutput.HideSelection = True
		Me.txtOutput.ReadOnly = False
		Me.txtOutput.Maxlength = 0
		Me.txtOutput.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtOutput.MultiLine = False
		Me.txtOutput.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtOutput.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtOutput.TabStop = True
		Me.txtOutput.Visible = True
		Me.txtOutput.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtOutput.Name = "txtOutput"
		udpTDTComputer.OcxState = CType(resources.GetObject("udpTDTComputer.OcxState"), System.Windows.Forms.AxHost.State)
		Me.udpTDTComputer.Location = New System.Drawing.Point(312, 16)
		Me.udpTDTComputer.Name = "udpTDTComputer"
		Me.Label4.Text = "Connect B2 DigOut on patch Panel to Trig on RA16  Connect A1 to Audio Monitor for Sound"
		Me.Label4.ForeColor = System.Drawing.Color.FromARGB(8, 0, 0)
		Me.Label4.Size = New System.Drawing.Size(257, 25)
		Me.Label4.Location = New System.Drawing.Point(16, 48)
		Me.Label4.TabIndex = 6
		Me.Label4.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label4.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label4.BackColor = System.Drawing.SystemColors.Control
		Me.Label4.Enabled = True
		Me.Label4.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label4.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label4.UseMnemonic = True
		Me.Label4.Visible = True
		Me.Label4.AutoSize = False
		Me.Label4.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label4.Name = "Label4"
		Me.TrigShape.BackStyle = Microsoft.VisualBasic.PowerPacks.BackStyle.Opaque
		Me.TrigShape.FillColor = System.Drawing.Color.Blue
		Me.TrigShape.FillStyle = Microsoft.VisualBasic.PowerPacks.FillStyle.Solid
		Me.TrigShape.Size = New System.Drawing.Size(33, 17)
		Me.TrigShape.Location = New System.Drawing.Point(240, 80)
		Me.TrigShape.Visible = False
		Me.TrigShape.BackColor = System.Drawing.SystemColors.Window
		Me.TrigShape.BorderColor = System.Drawing.SystemColors.WindowText
		Me.TrigShape.BorderStyle = System.Drawing.Drawing2D.DashStyle.Solid
		Me.TrigShape.BorderWidth = 1
		Me.TrigShape.Name = "TrigShape"
		Me.Label3.Text = "Text Received     Interval in Sec"
		Me.Label3.Size = New System.Drawing.Size(185, 17)
		Me.Label3.Location = New System.Drawing.Point(16, 80)
		Me.Label3.TabIndex = 5
		Me.Label3.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label3.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label3.BackColor = System.Drawing.SystemColors.Control
		Me.Label3.Enabled = True
		Me.Label3.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label3.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label3.UseMnemonic = True
		Me.Label3.Visible = True
		Me.Label3.AutoSize = False
		Me.Label3.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label3.Name = "Label3"
		Me.Label2.Text = "Text Received"
		Me.Label2.Size = New System.Drawing.Size(113, 17)
		Me.Label2.Location = New System.Drawing.Point(16, 8)
		Me.Label2.TabIndex = 4
		Me.Label2.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label2.BackColor = System.Drawing.SystemColors.Control
		Me.Label2.Enabled = True
		Me.Label2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label2.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label2.UseMnemonic = True
		Me.Label2.Visible = True
		Me.Label2.AutoSize = False
		Me.Label2.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label2.Name = "Label2"
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.TopCenter
		Me.Label1.Text = "Local UDP Port"
		Me.Label1.Size = New System.Drawing.Size(89, 17)
		Me.Label1.Location = New System.Drawing.Point(192, 8)
		Me.Label1.TabIndex = 2
		Me.Label1.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label1.BackColor = System.Drawing.SystemColors.Control
		Me.Label1.Enabled = True
		Me.Label1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label1.UseMnemonic = True
		Me.Label1.Visible = True
		Me.Label1.AutoSize = False
		Me.Label1.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label1.Name = "Label1"
		Me.Controls.Add(Quit)
		Me.Controls.Add(StopSend)
		Me.Controls.Add(StartSend)
		Me.Controls.Add(RPcoX1)
		Me.Controls.Add(TDevAccX1)
		Me.Controls.Add(lstOutput)
		Me.Controls.Add(BLocalPort)
		Me.Controls.Add(txtOutput)
		Me.Controls.Add(udpTDTComputer)
		Me.Controls.Add(Label4)
		Me.ShapeContainer1.Shapes.Add(TrigShape)
		Me.Controls.Add(Label3)
		Me.Controls.Add(Label2)
		Me.Controls.Add(Label1)
		Me.Controls.Add(ShapeContainer1)
		CType(Me.udpTDTComputer, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.TDevAccX1, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.RPcoX1, System.ComponentModel.ISupportInitialize).EndInit()
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class