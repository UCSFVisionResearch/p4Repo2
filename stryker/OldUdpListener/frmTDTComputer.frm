VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{735FF2F5-CC9A-4D67-BFC1-FD5FA6742027}#1.0#0"; "TDevAccX.ocx"
Object = "{D323A622-1D13-11D4-8858-444553540000}#1.0#0"; "RPcoX.ocx"
Begin VB.Form frmTDTComputer 
   AutoRedraw      =   -1  'True
   Caption         =   "TDT Computer"
   ClientHeight    =   7650
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   5715
   FillColor       =   &H00000008&
   LinkTopic       =   "Form1"
   ScaleHeight     =   7650
   ScaleWidth      =   5715
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Quit 
      Caption         =   "Quit"
      Height          =   615
      Left            =   3000
      TabIndex        =   9
      Top             =   1560
      Width           =   975
   End
   Begin VB.CommandButton StopSend 
      Caption         =   "StopSend"
      Height          =   615
      Left            =   1680
      TabIndex        =   8
      Top             =   1560
      Width           =   1095
   End
   Begin VB.CommandButton StartSend 
      Caption         =   "StartSend"
      Height          =   615
      Left            =   480
      TabIndex        =   7
      Top             =   1560
      Width           =   1095
   End
   Begin RPCOXLib.RPcoX RPcoX1 
      Left            =   4560
      Top             =   1560
      _Version        =   65536
      _ExtentX        =   1296
      _ExtentY        =   1296
      _StockProps     =   0
   End
   Begin TDEVACCXLib.TDevAccX TDevAccX1 
      Left            =   4680
      Top             =   840
      _Version        =   65536
      _ExtentX        =   873
      _ExtentY        =   873
      _StockProps     =   0
   End
   Begin VB.ListBox lstOutput 
      Height          =   5130
      Left            =   240
      TabIndex        =   3
      Top             =   2280
      Width           =   3855
   End
   Begin VB.TextBox BLocalPort 
      Height          =   285
      Left            =   3360
      TabIndex        =   1
      Text            =   "8938"
      Top             =   360
      Width           =   735
   End
   Begin VB.TextBox txtOutput 
      Height          =   285
      Left            =   240
      TabIndex        =   0
      Text            =   "Text Received"
      Top             =   360
      Width           =   2655
   End
   Begin MSWinsockLib.Winsock udpTDTComputer 
      Left            =   4680
      Top             =   240
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
   End
   Begin VB.Label Label4 
      Caption         =   "Connect B2 DigOut on patch Panel to Trig on RA16  Connect A1 to Audio Monitor for Sound"
      ForeColor       =   &H00000008&
      Height          =   375
      Left            =   240
      TabIndex        =   6
      Top             =   720
      Width           =   3855
   End
   Begin VB.Shape TrigShape 
      BackStyle       =   1  'Opaque
      FillColor       =   &H00FF0000&
      FillStyle       =   0  'Solid
      Height          =   255
      Left            =   3600
      Top             =   1200
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Label Label3 
      Caption         =   "Text Received     Interval in Sec"
      Height          =   255
      Left            =   240
      TabIndex        =   5
      Top             =   1200
      Width           =   2775
   End
   Begin VB.Label Label2 
      Caption         =   "Text Received"
      Height          =   255
      Left            =   240
      TabIndex        =   4
      Top             =   120
      Width           =   1695
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "Local UDP Port"
      Height          =   255
      Left            =   2880
      TabIndex        =   2
      Top             =   120
      Width           =   1335
   End
End
Attribute VB_Name = "frmTDTComputer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public Last As Single, iListSize As Integer, RPconnected As Integer, Running As Boolean
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Private Sub BLocalPort_Click()
udpTDTComputer.Close
udpTDTComputer.Bind Val(BLocalPort.Text)
End Sub
Private Sub Quit_Click()
Dim f As Form
For Each f In Forms
     Unload f
     Next
          End
End Sub
Private Sub StartSend_Click()
    If RPconnected <> 1 Then
    If RPcoX1.ConnectRZ5("GB", 1) Then
 '       MsgBox "Connection established"
        RPconnected = 1
    Else
        MsgBox "Error connecting to RZ5"
    End If
    End If
End Sub
Private Sub StopSend_Click()
    Running = False
End Sub
Private Sub Form_Load()
'Setup connection to TDTDevAcc
a = TDevAccX1.ConnectServer("Local")
If TDevAccX1.CheckServerConnection = 0 Then
    MsgBox ("TDevAccX1.CheckServerConnection: Open your TDT project and  Workbench is in Record or Preview Mode before restarting this VB program")
    End
Else
'    MsgBox ("TDevAccX1.CheckServerConnection succeded")
' The control's name is udpTDTComputer.
    udpTDTComputer.Bind Val(BLocalPort.Text)               ' Bind to the default local port.
    iListSize = 0
End If
   
TrigShape.Visible = True
TrigShape.FillColor = &HFF0000       'blue
    RPconnected = 0

If RPconnected = 0 Then
    RPconnected = RPcoX1.ConnectRZ5("GB", 1)
End If
If RPconnected <> 1 Then
        MsgBox "Error connecting to RZ5"
End If
    
End Sub

Private Sub udpTDTComputer_DataArrival _
(ByVal bytesTotal As Long)
Dim strData As String

If RPconnected = 1 Then 'Get the data only if the input is connected, RPvds file must be in Run mode
    TrigShape.FillColor = &HFF00&        'green
    TrigShape.Visible = True
    TrigShape.Refresh
    
    
    udpTDTComputer.GetData strData
    txtOutput.Text = strData
    lstOutput.AddItem strData
    
    'Write values and indices for next epoc
    
    If (strData <> "A") Then
        ' do if not movie (sends "A")
        c = Split(strData, " ", -1)
        cond = Int(c(0))
        a_val = Val(c(1))
        b_val = Val(c(2))
        c_val = Val(c(3))
        a_ind = Int(c(4))
        b_ind = Int(c(5))
        c_ind = Int(c(6))
        EZ3 = TDevAccX1.SetTargetVal("Amp1.CondNum", cond)
        EZ1 = TDevAccX1.SetTargetVal("Amp1.A_value", a_val)
        EZ2 = TDevAccX1.SetTargetVal("Amp1.B_value", b_val)
        EZ6 = TDevAccX1.SetTargetVal("Amp1.C_value", c_val)
        EZ4 = TDevAccX1.SetTargetVal("Amp1.A_index", a_ind)
        EZ5 = TDevAccX1.SetTargetVal("Amp1.B_index", b_ind)
        EZ7 = TDevAccX1.SetTargetVal("Amp1.C_index", c_ind)
        
    End If
    
  
    
    TrigShape.Visible = False
    TrigShape.Refresh
    
    'we will probabaly stop this if no need for debugging
    iListSize = iListSize + 1
    If iListSize > 25 Then lstOutput.RemoveItem 0
Else
        MsgBox "Error connecting to RZ5"
End If 'RPconnected
End Sub
