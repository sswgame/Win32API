namespace WindowsFormsApp
{
    partial class SpriteEditor
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SpriteEditor));
            this.spriteInfoTablePanel = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnFlipImage = new System.Windows.Forms.Button();
            this.txtFrameIndex = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.button11 = new System.Windows.Forms.Button();
            this.txtDuration = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.txtAnimName = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.txtScale = new System.Windows.Forms.Label();
            this.button10 = new System.Windows.Forms.Button();
            this.btnStart = new System.Windows.Forms.Button();
            this.button9 = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.backGroundColorBox = new System.Windows.Forms.PictureBox();
            this.label9 = new System.Windows.Forms.Label();
            this.txtGreen = new System.Windows.Forms.TextBox();
            this.txtBlue = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtRed = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.txtCenterY = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.txtCenterX = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.spriteHeight = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.spriteWidth = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.posY = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.posX = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.animationBox = new System.Windows.Forms.PictureBox();
            this.spriteBox = new System.Windows.Forms.PictureBox();
            this.AnimTreeView = new System.Windows.Forms.TreeView();
            this.panel3 = new System.Windows.Forms.Panel();
            this.spriteSheetBox = new System.Windows.Forms.PictureBox();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.fileMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.ExportMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.StatusPos = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.ArrowTool = new System.Windows.Forms.ToolStripButton();
            this.ColorPicker = new System.Windows.Forms.ToolStripButton();
            this.CaptureTool = new System.Windows.Forms.ToolStripButton();
            this.panel2 = new System.Windows.Forms.Panel();
            this.AnimTimer = new System.Windows.Forms.Timer(this.components);
            this.spriteInfoTablePanel.SuspendLayout();
            this.panel1.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.backGroundColorBox)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.animationBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.spriteBox)).BeginInit();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.spriteSheetBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).BeginInit();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.menuStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.toolStrip.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // spriteInfoTablePanel
            // 
            this.spriteInfoTablePanel.ColumnCount = 2;
            this.spriteInfoTablePanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.spriteInfoTablePanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.spriteInfoTablePanel.Controls.Add(this.panel1, 1, 0);
            this.spriteInfoTablePanel.Controls.Add(this.animationBox, 0, 1);
            this.spriteInfoTablePanel.Controls.Add(this.spriteBox, 1, 1);
            this.spriteInfoTablePanel.Controls.Add(this.AnimTreeView, 0, 0);
            this.spriteInfoTablePanel.Location = new System.Drawing.Point(-1, 0);
            this.spriteInfoTablePanel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.spriteInfoTablePanel.Name = "spriteInfoTablePanel";
            this.spriteInfoTablePanel.RowCount = 2;
            this.spriteInfoTablePanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.spriteInfoTablePanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.spriteInfoTablePanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.spriteInfoTablePanel.Size = new System.Drawing.Size(559, 938);
            this.spriteInfoTablePanel.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox4);
            this.panel1.Controls.Add(this.txtScale);
            this.panel1.Controls.Add(this.button10);
            this.panel1.Controls.Add(this.btnStart);
            this.panel1.Controls.Add(this.button9);
            this.panel1.Controls.Add(this.groupBox3);
            this.panel1.Controls.Add(this.button5);
            this.panel1.Controls.Add(this.button6);
            this.panel1.Controls.Add(this.groupBox2);
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Location = new System.Drawing.Point(282, 4);
            this.panel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(273, 461);
            this.panel1.TabIndex = 3;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.btnFlipImage);
            this.groupBox4.Controls.Add(this.txtFrameIndex);
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Controls.Add(this.button11);
            this.groupBox4.Controls.Add(this.txtDuration);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Controls.Add(this.txtAnimName);
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Location = new System.Drawing.Point(9, 266);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox4.Size = new System.Drawing.Size(264, 116);
            this.groupBox4.TabIndex = 17;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "애니메이션 정보";
            // 
            // btnFlipImage
            // 
            this.btnFlipImage.Location = new System.Drawing.Point(173, 48);
            this.btnFlipImage.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnFlipImage.Name = "btnFlipImage";
            this.btnFlipImage.Size = new System.Drawing.Size(72, 29);
            this.btnFlipImage.TabIndex = 23;
            this.btnFlipImage.Text = "반전";
            this.btnFlipImage.UseVisualStyleBackColor = true;
            this.btnFlipImage.Click += new System.EventHandler(this.OnFlipImage);
            // 
            // txtFrameIndex
            // 
            this.txtFrameIndex.Location = new System.Drawing.Point(91, 81);
            this.txtFrameIndex.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtFrameIndex.Name = "txtFrameIndex";
            this.txtFrameIndex.ReadOnly = true;
            this.txtFrameIndex.Size = new System.Drawing.Size(68, 25);
            this.txtFrameIndex.TabIndex = 22;
            this.txtFrameIndex.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(7, 86);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(78, 15);
            this.label12.TabIndex = 21;
            this.label12.Text = "frameIndex";
            // 
            // button11
            // 
            this.button11.Location = new System.Drawing.Point(173, 81);
            this.button11.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(72, 29);
            this.button11.TabIndex = 20;
            this.button11.Text = "변경";
            this.button11.UseVisualStyleBackColor = true;
            this.button11.Click += new System.EventHandler(this.OnUpdateAnimInfo);
            // 
            // txtDuration
            // 
            this.txtDuration.Location = new System.Drawing.Point(91, 48);
            this.txtDuration.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtDuration.Name = "txtDuration";
            this.txtDuration.Size = new System.Drawing.Size(68, 25);
            this.txtDuration.TabIndex = 19;
            this.txtDuration.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(9, 51);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(61, 15);
            this.label11.TabIndex = 18;
            this.label11.Text = "Duration";
            // 
            // txtAnimName
            // 
            this.txtAnimName.Location = new System.Drawing.Point(91, 18);
            this.txtAnimName.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtAnimName.Name = "txtAnimName";
            this.txtAnimName.Size = new System.Drawing.Size(153, 25);
            this.txtAnimName.TabIndex = 14;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(14, 21);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(37, 15);
            this.label10.TabIndex = 13;
            this.label10.Text = "이름";
            // 
            // txtScale
            // 
            this.txtScale.AutoSize = true;
            this.txtScale.Location = new System.Drawing.Point(16, 432);
            this.txtScale.Name = "txtScale";
            this.txtScale.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.txtScale.Size = new System.Drawing.Size(59, 15);
            this.txtScale.TabIndex = 16;
            this.txtScale.Text = "1x 배율";
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(103, 426);
            this.button10.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(72, 29);
            this.button10.TabIndex = 15;
            this.button10.Text = "확대";
            this.button10.UseVisualStyleBackColor = true;
            this.button10.Click += new System.EventHandler(this.OnIncreaseRatio);
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(18, 390);
            this.btnStart.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(72, 29);
            this.btnStart.TabIndex = 17;
            this.btnStart.Text = "재생";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.OnExecuteAnimation);
            // 
            // button9
            // 
            this.button9.Location = new System.Drawing.Point(182, 426);
            this.button9.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(72, 29);
            this.button9.TabIndex = 14;
            this.button9.Text = "축소";
            this.button9.UseVisualStyleBackColor = true;
            this.button9.Click += new System.EventHandler(this.OnDecreaseRatio);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.backGroundColorBox);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.txtGreen);
            this.groupBox3.Controls.Add(this.txtBlue);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.txtRed);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Location = new System.Drawing.Point(7, 200);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox3.Size = new System.Drawing.Size(266, 58);
            this.groupBox3.TabIndex = 8;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "배경색";
            // 
            // backGroundColorBox
            // 
            this.backGroundColorBox.Location = new System.Drawing.Point(223, 20);
            this.backGroundColorBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.backGroundColorBox.Name = "backGroundColorBox";
            this.backGroundColorBox.Size = new System.Drawing.Size(24, 30);
            this.backGroundColorBox.TabIndex = 6;
            this.backGroundColorBox.TabStop = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(151, 28);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(17, 15);
            this.label9.TabIndex = 5;
            this.label9.Text = "B";
            // 
            // txtGreen
            // 
            this.txtGreen.Location = new System.Drawing.Point(99, 24);
            this.txtGreen.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtGreen.Name = "txtGreen";
            this.txtGreen.Size = new System.Drawing.Size(43, 25);
            this.txtGreen.TabIndex = 4;
            this.txtGreen.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtGreen.TextChanged += new System.EventHandler(this.TxtGreen_TextChanged);
            // 
            // txtBlue
            // 
            this.txtBlue.Location = new System.Drawing.Point(173, 24);
            this.txtBlue.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtBlue.Name = "txtBlue";
            this.txtBlue.Size = new System.Drawing.Size(43, 25);
            this.txtBlue.TabIndex = 3;
            this.txtBlue.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtBlue.TextChanged += new System.EventHandler(this.TxtBlue_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(77, 28);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(18, 15);
            this.label5.TabIndex = 2;
            this.label5.Text = "G";
            // 
            // txtRed
            // 
            this.txtRed.Location = new System.Drawing.Point(26, 24);
            this.txtRed.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtRed.Name = "txtRed";
            this.txtRed.Size = new System.Drawing.Size(43, 25);
            this.txtRed.TabIndex = 1;
            this.txtRed.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtRed.TextChanged += new System.EventHandler(this.TxtRed_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(5, 28);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(17, 15);
            this.label6.TabIndex = 0;
            this.label6.Text = "R";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(182, 390);
            this.button5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(72, 29);
            this.button5.TabIndex = 15;
            this.button5.Text = "삭제";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.OnRemoveAnimation);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(103, 390);
            this.button6.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(72, 29);
            this.button6.TabIndex = 16;
            this.button6.Text = "추가";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.OnAddAnimation);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button4);
            this.groupBox2.Controls.Add(this.button3);
            this.groupBox2.Controls.Add(this.button2);
            this.groupBox2.Controls.Add(this.button1);
            this.groupBox2.Controls.Add(this.txtCenterY);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.txtCenterX);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Location = new System.Drawing.Point(3, 101);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox2.Size = new System.Drawing.Size(266, 91);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "중심 좌표(좌상단 기준 변화량)";
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(219, 15);
            this.button4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(34, 29);
            this.button4.TabIndex = 7;
            this.button4.Text = "+";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.OnIncreaseCenterX);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(219, 51);
            this.button3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(34, 29);
            this.button3.TabIndex = 6;
            this.button3.Text = "+";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.OnIncreaseCenterY);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(178, 51);
            this.button2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(34, 29);
            this.button2.TabIndex = 5;
            this.button2.Text = "-";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.OnDecreaseCenterY);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(178, 15);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(34, 29);
            this.button1.TabIndex = 4;
            this.button1.Text = "-";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.OnDecreaseCenterX);
            // 
            // txtCenterY
            // 
            this.txtCenterY.Location = new System.Drawing.Point(57, 51);
            this.txtCenterY.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtCenterY.Name = "txtCenterY";
            this.txtCenterY.Size = new System.Drawing.Size(114, 25);
            this.txtCenterY.TabIndex = 3;
            this.txtCenterY.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtCenterY.TextChanged += new System.EventHandler(this.txtCenterY_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(7, 55);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(50, 15);
            this.label7.TabIndex = 2;
            this.label7.Text = "y 좌표";
            // 
            // txtCenterX
            // 
            this.txtCenterX.Location = new System.Drawing.Point(57, 18);
            this.txtCenterX.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.txtCenterX.Name = "txtCenterX";
            this.txtCenterX.Size = new System.Drawing.Size(114, 25);
            this.txtCenterX.TabIndex = 1;
            this.txtCenterX.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtCenterX.TextChanged += new System.EventHandler(this.txtCenterX_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(7, 21);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(51, 15);
            this.label8.TabIndex = 0;
            this.label8.Text = "x 좌표";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.spriteHeight);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.spriteWidth);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.posY);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.posX);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(3, 4);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox1.Size = new System.Drawing.Size(270, 90);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "스프라이트 정보";
            // 
            // spriteHeight
            // 
            this.spriteHeight.Location = new System.Drawing.Point(183, 52);
            this.spriteHeight.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.spriteHeight.Name = "spriteHeight";
            this.spriteHeight.ReadOnly = true;
            this.spriteHeight.Size = new System.Drawing.Size(46, 25);
            this.spriteHeight.TabIndex = 7;
            this.spriteHeight.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(133, 56);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 15);
            this.label4.TabIndex = 6;
            this.label4.Text = "높이";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // spriteWidth
            // 
            this.spriteWidth.Location = new System.Drawing.Point(183, 19);
            this.spriteWidth.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.spriteWidth.Name = "spriteWidth";
            this.spriteWidth.ReadOnly = true;
            this.spriteWidth.Size = new System.Drawing.Size(46, 25);
            this.spriteWidth.TabIndex = 5;
            this.spriteWidth.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(133, 22);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 15);
            this.label3.TabIndex = 4;
            this.label3.Text = "넓이";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // posY
            // 
            this.posY.Location = new System.Drawing.Point(54, 52);
            this.posY.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.posY.Name = "posY";
            this.posY.ReadOnly = true;
            this.posY.Size = new System.Drawing.Size(46, 25);
            this.posY.TabIndex = 3;
            this.posY.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "y 좌표";
            // 
            // posX
            // 
            this.posX.Location = new System.Drawing.Point(54, 19);
            this.posX.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.posX.Name = "posX";
            this.posX.ReadOnly = true;
            this.posX.Size = new System.Drawing.Size(46, 25);
            this.posX.TabIndex = 1;
            this.posX.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(51, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "x 좌표";
            // 
            // animationBox
            // 
            this.animationBox.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.animationBox.Location = new System.Drawing.Point(3, 473);
            this.animationBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.animationBox.Name = "animationBox";
            this.animationBox.Size = new System.Drawing.Size(272, 461);
            this.animationBox.TabIndex = 0;
            this.animationBox.TabStop = false;
            // 
            // spriteBox
            // 
            this.spriteBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.spriteBox.Location = new System.Drawing.Point(282, 473);
            this.spriteBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.spriteBox.Name = "spriteBox";
            this.spriteBox.Size = new System.Drawing.Size(273, 461);
            this.spriteBox.TabIndex = 1;
            this.spriteBox.TabStop = false;
            // 
            // AnimTreeView
            // 
            this.AnimTreeView.LabelEdit = true;
            this.AnimTreeView.Location = new System.Drawing.Point(3, 4);
            this.AnimTreeView.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.AnimTreeView.Name = "AnimTreeView";
            this.AnimTreeView.Size = new System.Drawing.Size(271, 460);
            this.AnimTreeView.TabIndex = 4;
            this.AnimTreeView.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.AnimTreeView_AfterLabelEdit);
            this.AnimTreeView.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.AnimTreeView_ItemDrag);
            this.AnimTreeView.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.OnTreeViewClick);
            // 
            // panel3
            // 
            this.panel3.AutoScroll = true;
            this.panel3.Controls.Add(this.spriteSheetBox);
            this.panel3.Location = new System.Drawing.Point(0, 48);
            this.panel3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(795, 890);
            this.panel3.TabIndex = 2;
            // 
            // spriteSheetBox
            // 
            this.spriteSheetBox.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.spriteSheetBox.Location = new System.Drawing.Point(3, 4);
            this.spriteSheetBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.spriteSheetBox.Name = "spriteSheetBox";
            this.spriteSheetBox.Size = new System.Drawing.Size(693, 722);
            this.spriteSheetBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.spriteSheetBox.TabIndex = 2;
            this.spriteSheetBox.TabStop = false;
            this.spriteSheetBox.MouseClick += new System.Windows.Forms.MouseEventHandler(this.OnMouseClick);
            // 
            // splitContainer
            // 
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 28);
            this.splitContainer.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.splitContainer.Name = "splitContainer";
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.panel3);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.spriteInfoTablePanel);
            this.splitContainer.Size = new System.Drawing.Size(1361, 968);
            this.splitContainer.SplitterDistance = 805;
            this.splitContainer.SplitterWidth = 5;
            this.splitContainer.TabIndex = 1;
            // 
            // fileMenu
            // 
            this.fileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.OpenMenu,
            this.loadToolStripMenuItem,
            this.SaveMenu,
            this.ExportMenu});
            this.fileMenu.Name = "fileMenu";
            this.fileMenu.Size = new System.Drawing.Size(46, 24);
            this.fileMenu.Text = "File";
            // 
            // OpenMenu
            // 
            this.OpenMenu.Name = "OpenMenu";
            this.OpenMenu.Size = new System.Drawing.Size(189, 26);
            this.OpenMenu.Text = "Open";
            this.OpenMenu.Click += new System.EventHandler(this.OnFileOpen);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(189, 26);
            this.loadToolStripMenuItem.Text = "Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.OnLoad);
            // 
            // SaveMenu
            // 
            this.SaveMenu.Name = "SaveMenu";
            this.SaveMenu.Size = new System.Drawing.Size(189, 26);
            this.SaveMenu.Text = "Save";
            this.SaveMenu.Click += new System.EventHandler(this.OnSave);
            // 
            // ExportMenu
            // 
            this.ExportMenu.Name = "ExportMenu";
            this.ExportMenu.Size = new System.Drawing.Size(189, 26);
            this.ExportMenu.Text = "Export to XML";
            this.ExportMenu.Click += new System.EventHandler(this.OnExportToXML);
            // 
            // menuStrip
            // 
            this.menuStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileMenu});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Padding = new System.Windows.Forms.Padding(7, 2, 0, 2);
            this.menuStrip.Size = new System.Drawing.Size(1361, 28);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "menuStrip1";
            // 
            // StatusPos
            // 
            this.StatusPos.Name = "StatusPos";
            this.StatusPos.Size = new System.Drawing.Size(52, 20);
            this.StatusPos.Text = "좌표 : ";
            // 
            // statusStrip
            // 
            this.statusStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusPos});
            this.statusStrip.Location = new System.Drawing.Point(0, 970);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Padding = new System.Windows.Forms.Padding(1, 0, 16, 0);
            this.statusStrip.Size = new System.Drawing.Size(1361, 26);
            this.statusStrip.TabIndex = 3;
            this.statusStrip.Text = "statusStrip1";
            // 
            // toolStrip
            // 
            this.toolStrip.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ArrowTool,
            this.ColorPicker,
            this.CaptureTool});
            this.toolStrip.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
            this.toolStrip.Location = new System.Drawing.Point(0, 0);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.Size = new System.Drawing.Size(792, 44);
            this.toolStrip.TabIndex = 1;
            this.toolStrip.Text = "toolStrip";
            // 
            // ArrowTool
            // 
            this.ArrowTool.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ArrowTool.Image = ((System.Drawing.Image)(resources.GetObject("ArrowTool.Image")));
            this.ArrowTool.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ArrowTool.Name = "ArrowTool";
            this.ArrowTool.Size = new System.Drawing.Size(29, 24);
            this.ArrowTool.Text = "toolStripButton1";
            this.ArrowTool.Click += new System.EventHandler(this.OnArrowTool);
            // 
            // ColorPicker
            // 
            this.ColorPicker.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ColorPicker.Image = ((System.Drawing.Image)(resources.GetObject("ColorPicker.Image")));
            this.ColorPicker.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ColorPicker.Name = "ColorPicker";
            this.ColorPicker.Size = new System.Drawing.Size(29, 24);
            this.ColorPicker.Text = "toolStripButton3";
            this.ColorPicker.Click += new System.EventHandler(this.OnColorPickerTool);
            // 
            // CaptureTool
            // 
            this.CaptureTool.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.CaptureTool.Image = ((System.Drawing.Image)(resources.GetObject("CaptureTool.Image")));
            this.CaptureTool.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.CaptureTool.Name = "CaptureTool";
            this.CaptureTool.Size = new System.Drawing.Size(29, 24);
            this.CaptureTool.Text = "toolStripButton1";
            this.CaptureTool.Click += new System.EventHandler(this.OnCaptureTool);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.toolStrip);
            this.panel2.Location = new System.Drawing.Point(3, 30);
            this.panel2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(792, 44);
            this.panel2.TabIndex = 2;
            // 
            // SpriteEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1361, 996);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.splitContainer);
            this.Controls.Add(this.menuStrip);
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.Name = "SpriteEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "SpriteEditor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnClosing);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
            this.spriteInfoTablePanel.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.backGroundColorBox)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.animationBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.spriteBox)).EndInit();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.spriteSheetBox)).EndInit();
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).EndInit();
            this.splitContainer.ResumeLayout(false);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel spriteInfoTablePanel;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox txtFrameIndex;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.TextBox txtDuration;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox txtAnimName;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label txtScale;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.PictureBox backGroundColorBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox txtGreen;
        private System.Windows.Forms.TextBox txtBlue;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtRed;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox txtCenterY;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtCenterX;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox spriteHeight;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox spriteWidth;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox posY;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox posX;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox animationBox;
        private System.Windows.Forms.PictureBox spriteBox;
        private System.Windows.Forms.TreeView AnimTreeView;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.PictureBox spriteSheetBox;
        private System.Windows.Forms.SplitContainer splitContainer;
        private System.Windows.Forms.ToolStripMenuItem fileMenu;
        private System.Windows.Forms.ToolStripMenuItem OpenMenu;
        private System.Windows.Forms.ToolStripMenuItem SaveMenu;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripStatusLabel StatusPos;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripButton ArrowTool;
        private System.Windows.Forms.ToolStripButton ColorPicker;
        private System.Windows.Forms.ToolStripButton CaptureTool;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Timer AnimTimer;
        private System.Windows.Forms.ToolStripMenuItem ExportMenu;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.Button btnFlipImage;
    }
}

