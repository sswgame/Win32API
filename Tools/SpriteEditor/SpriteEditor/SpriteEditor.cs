using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using WindowsFormsApp.Logic;

namespace WindowsFormsApp
{
    public partial class SpriteEditor : Form
    {
        private readonly Dictionary<string, List<Sprite>> m_mapAnimations;
        private readonly bool[]                           m_arrToolSelected;

        private Bitmap m_backBuffer;
        private Point  m_backBufferCenter;

        private Color  m_backGroundColor;
        private Bitmap m_croppedBitmap;

        private readonly float m_deltaTime;

        private Bitmap m_animBackBuffer;
        private Point  m_animBackBufferCenter;

        private Sprite       m_currentAnimSprite;
        private List<Sprite> m_currentAnimationFrames;
        private int          m_frameIndex;
        private bool         m_isFinished;
        private float        m_accumulatedTime;

        private Sprite m_currentSprite;
        private Point  m_endPoint;
        private bool   m_isAnimationStarted;

        private bool m_isCapture;
        private bool m_isZooming;

        private string m_spriteSheetName;
        private Bitmap m_originalSheetBitmap;
        private Bitmap m_resizeSheetBitmap;

        private float m_sheetScale;
        private int   m_spriteScale;
        private Point m_startPoint;

        private bool m_isSaved;

        public SpriteEditor()
        {
            InitializeComponent();
            spriteSheetBox.MouseWheel += OnMouseWheel;
            spriteSheetBox.MouseMove  += OnMouseMove;
            spriteSheetBox.MouseUp    += OnMouseUp;
            spriteSheetBox.MouseDown  += OnMouseDown;

            m_deltaTime     =  (float)AnimTimer.Interval * TimeSpan.TicksPerMillisecond / TimeSpan.TicksPerSecond;
            AnimTimer.Tick += TimerEvent;

            m_mapAnimations = new Dictionary<string, List<Sprite>>();
            m_arrToolSelected  = new bool[Enum.GetValues(typeof(Tool)).Length];

            m_sheetScale  = 1.0f;
            m_spriteScale = 1;
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            StatusPos.Text = $@"좌표 : {e.X}, {e.Y}";

            if (m_isCapture)
                DrawSelectionBox(GetRoundPoint(e.Location));
        }

        // Round the point to the nearest unscaled pixel location.
        private Point GetRoundPoint(Point point)
        {
            int x = (int)(m_sheetScale * (int)(point.X / m_sheetScale));
            int y = (int)(m_sheetScale * (int)(point.Y / m_sheetScale));

            return new Point(x, y);
        }

        private void OnMouseDown(object sender, MouseEventArgs e)
        {
            if (!m_arrToolSelected[Tool.Capture.GetHashCode()])
                return;

            m_isCapture     = true;
            m_startPoint    = GetRoundPoint(e.Location);
            m_croppedBitmap = new Bitmap(m_resizeSheetBitmap);

            DrawSelectionBox(m_startPoint);
        }

        private void DrawSelectionBox(Point endPoint)
        {
            m_endPoint = endPoint;
            if (m_endPoint.X < 0) m_endPoint.X = 0;
            if (m_endPoint.Y < 0) m_endPoint.Y = 0;

            if (m_endPoint.X >= m_resizeSheetBitmap.Width) m_endPoint.X  = m_resizeSheetBitmap.Width  - 1;
            if (m_endPoint.Y >= m_resizeSheetBitmap.Height) m_endPoint.Y = m_resizeSheetBitmap.Height - 1;

            int x      = Math.Min(m_startPoint.X, m_endPoint.X);
            int y      = Math.Min(m_startPoint.Y, m_endPoint.Y);
            int width  = Math.Abs(m_startPoint.X - m_endPoint.X);
            int height = Math.Abs(m_startPoint.Y - m_endPoint.Y);

            using (var g = Graphics.FromImage(m_croppedBitmap))
            {
                g.DrawImage(m_resizeSheetBitmap, 0, 0);
                g.DrawRectangle(Pens.Green, x, y, width, height);
            }

            spriteSheetBox.Image = m_croppedBitmap;
            spriteSheetBox.Refresh();
        }

        private void OnMouseUp(object sender, MouseEventArgs e)
        {
            if (!m_isCapture)
                return;

            m_isCapture = false;

            if (m_backGroundColor.IsEmpty)
            {
                MessageBox.Show(@"Select Background Color first");
                return;
            }

            int x      = (int)(Math.Min(m_startPoint.X, m_endPoint.X)  / m_sheetScale);
            int y      = (int)(Math.Min(m_startPoint.Y, m_endPoint.Y)  / m_sheetScale);
            int width  = (int)(Math.Abs(m_startPoint.X - m_endPoint.X) / m_sheetScale);
            int height = (int)(Math.Abs(m_startPoint.Y - m_endPoint.Y) / m_sheetScale);

            if (width  == 0
             || height == 0)
                return;

            var  sprite      = new Sprite();
            var  leftTop     = new Point(x, y);
            var  rightBottom = new Point(x + width, y + height);
            bool result      = sprite.FindWithRange(m_originalSheetBitmap, leftTop, rightBottom, m_backGroundColor);
            if (false == result)
                return;

            m_currentSprite = sprite;

            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            DisplayInfo();
        }

        private void OnMouseWheel(object sender, MouseEventArgs e)
        {
            if (m_originalSheetBitmap == null)
                return;

            if (!m_isZooming)
                return;

            if (e.Delta > 0)
            {
                ++m_sheetScale;
            }
            else
            {
                --m_sheetScale;
                if (m_sheetScale <= 0) m_sheetScale = 1.0f;
            }

            m_resizeSheetBitmap   = DrawBitmap(m_originalSheetBitmap, (int)m_sheetScale);
            spriteSheetBox.Image = m_resizeSheetBitmap;
            m_isZooming           = false;
        }

        private void OnFileOpen(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter = @"Image files (*.png;*.jpeg;*.bmp)|*.png;*.jpeg;*.bmp|All files (*.*)|*.*",
                InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            };

            var result = DialogResult.OK;
            if (false == m_isSaved && m_mapAnimations.Count > 0)
            {
                result = MessageBox.Show(@"The document has been modified. Do you want to save to file?",
                                         string.Empty, MessageBoxButtons.YesNoCancel);

                switch (result)
                {
                    case DialogResult.OK:
                        m_isSaved = SaveData();
                        break;
                    case DialogResult.Cancel:
                        return;
                }
            }

            if (openFileDialog.ShowDialog() != result)
                return;

            var image = Image.FromFile(openFileDialog.FileName);

            m_spriteSheetName     = openFileDialog.FileName;
            m_originalSheetBitmap = new Bitmap(image);
            m_resizeSheetBitmap   = m_originalSheetBitmap;

            spriteSheetBox.Width  = m_originalSheetBitmap.Width;
            spriteSheetBox.Height = m_originalSheetBitmap.Height;
            spriteSheetBox.Image  = m_originalSheetBitmap;

            m_backBuffer       = new Bitmap(spriteBox.Width, spriteBox.Height);
            m_backBufferCenter = new Point(m_backBuffer.Width / 2, m_backBuffer.Height / 2);

            m_animBackBuffer       = new Bitmap(animationBox.Width, animationBox.Height);
            m_animBackBufferCenter = new Point(m_animBackBuffer.Width / 2, m_animBackBuffer.Height / 2);
        }

        private void OnArrowTool(object sender, EventArgs e)
        {
            Array.Clear(m_arrToolSelected, 0, m_arrToolSelected.Length);
            m_arrToolSelected[Tool.Arrow.GetHashCode()] = true;
        }

        private void OnColorPickerTool(object sender, EventArgs e)
        {
            Array.Clear(m_arrToolSelected, 0, m_arrToolSelected.Length);
            m_arrToolSelected[Tool.ColorPick.GetHashCode()] = true;
        }

        private void OnCaptureTool(object sender, EventArgs e)
        {
            Array.Clear(m_arrToolSelected, 0, m_arrToolSelected.Length);
            m_arrToolSelected[Tool.Capture.GetHashCode()] = true;
        }

        private void OnMouseClick(object sender, MouseEventArgs e)
        {
            if (null == m_originalSheetBitmap)
                return;

            if (m_arrToolSelected[Tool.Arrow.GetHashCode()])
                SelectSprite(e.Location);
            else if (m_arrToolSelected[Tool.ColorPick.GetHashCode()])
                SelectBackgroundColor(e.Location);
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Modifiers == Keys.Control)
                m_isZooming = true;
        }

        private void SelectSprite(Point mousePos)
        {
            if (m_backGroundColor.IsEmpty)
            {
                MessageBox.Show(@"select backGroundColor first");
                return;
            }

            if (m_sheetScale > 1.0f)
            {
                mousePos.X /= (int)m_sheetScale;
                mousePos.Y /= (int)m_sheetScale;
            }

            var  sprite = new Sprite();
            bool result = sprite.FindWithDFS(m_originalSheetBitmap, mousePos, m_backGroundColor);
            if (false == result)
                return;

            m_currentSprite = sprite;

            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            DisplayInfo();
        }

        private void DrawSprite(Bitmap bitmap, Point centerPos, int scale)
        {
            bitmap = DrawBitmap(bitmap, scale);

            var spriteRectangle = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            var destRectangle   = new Rectangle(m_backBufferCenter.X - centerPos.X * scale,
                                                m_backBufferCenter.Y - centerPos.Y * scale,
                                                bitmap.Width,
                                                bitmap.Height);

            using (var g = Graphics.FromImage(m_backBuffer))
            {
                g.PixelOffsetMode   = PixelOffsetMode.HighQuality;
                g.InterpolationMode = InterpolationMode.NearestNeighbor;
                g.Clear(Color.Transparent);
                g.DrawImage(bitmap, destRectangle, spriteRectangle, GraphicsUnit.Pixel);
            }

            m_backBuffer = DrawCenterLine(m_backBuffer, m_backBufferCenter, scale);
            m_backBuffer.MakeTransparent(m_backGroundColor);
            spriteBox.Image = m_backBuffer;
        }

        private Bitmap DrawBitmap(Bitmap bitmap, int scale)
        {
            int scaleWidth  = scale * bitmap.Width;
            int scaleHeight = scale * bitmap.Height;

            var scaledBitmap  = new Bitmap(scaleWidth, scaleHeight);
            var srcRectangle  = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            var destRectangle = new Rectangle(0, 0, scaleWidth, scaleHeight);

            using (var g = Graphics.FromImage(scaledBitmap))
            {
                g.PixelOffsetMode   = PixelOffsetMode.HighQuality;
                g.InterpolationMode = InterpolationMode.NearestNeighbor;
                g.DrawImage(bitmap, destRectangle, srcRectangle, GraphicsUnit.Pixel);
            }

            return scaledBitmap;
        }

        private Bitmap DrawCenterLine(Bitmap bitmap, Point centerPos, int scale)
        {
            var centerLineBitmap = new Bitmap(bitmap);
            using (var g = Graphics.FromImage(centerLineBitmap))
            {
                for (int i = centerPos.X; i >= 0; i -= 2 * scale)
                    g.FillRectangle(Brushes.Green, new Rectangle(i, centerPos.Y, scale, scale));

                for (int i = centerPos.X; i <= bitmap.Width; i += 2 * scale)
                    g.FillRectangle(Brushes.Green, new Rectangle(i, centerPos.Y, scale, scale));

                for (int i = centerPos.Y; i >= 0; i -= 2 * scale)
                    g.FillRectangle(Brushes.Green, new Rectangle(centerPos.X, i, scale, scale));

                for (int i = centerPos.Y; i <= bitmap.Height; i += 2 * scale)
                    g.FillRectangle(Brushes.Green, new Rectangle(centerPos.X, i, scale, scale));
            }

            return centerLineBitmap;
        }

        private void DisplayInfo()
        {
            spriteWidth.Text  = m_currentSprite.Width.ToString();
            spriteHeight.Text = m_currentSprite.Height.ToString();

            posX.Text = m_currentSprite.LeftTop.X.ToString();
            posY.Text = m_currentSprite.LeftTop.Y.ToString();

            txtCenterX.Text = m_currentSprite.CenterPos.X.ToString();
            txtCenterY.Text = m_currentSprite.CenterPos.Y.ToString();
        }

        private void OnIncreaseRatio(object sender, EventArgs e)
        {
            ++m_spriteScale;
            txtScale.Text = $@"{m_spriteScale} x 배율";

            if (m_currentSprite == null) return;
            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);

            if (m_currentAnimSprite == null) return;
            DrawAnimSprite(m_currentAnimSprite.Bitmap, m_currentAnimSprite.CenterPos, m_spriteScale);
        }

        private void OnDecreaseRatio(object sender, EventArgs e)
        {
            --m_spriteScale;
            m_spriteScale = (m_spriteScale > 0)? m_spriteScale : 1;
            
            txtScale.Text = $@"{m_spriteScale} x 배율";

            if (m_currentSprite == null) return;
            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);

            if (m_currentAnimSprite == null) return;
            DrawAnimSprite(m_currentAnimSprite.Bitmap, m_currentAnimSprite.CenterPos, m_spriteScale);
        }

        private void OnDecreaseCenterX(object sender, EventArgs e)
        {
            if (m_currentSprite == null) return;

            AddCenterPos(m_currentSprite, -1, 0);
            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            txtCenterX.Text = m_currentSprite.CenterPos.X.ToString();
        }

        private void OnIncreaseCenterX(object sender, EventArgs e)
        {
            if (m_currentSprite == null) return;
               
            AddCenterPos(m_currentSprite, 1, 0);
            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            txtCenterX.Text = m_currentSprite.CenterPos.X.ToString();
        }

        private void OnDecreaseCenterY(object sender, EventArgs e)
        {
            if (m_currentSprite == null) return;
               
            AddCenterPos(m_currentSprite, 0, -1);
            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            txtCenterY.Text = m_currentSprite.CenterPos.Y.ToString();
        }

        private void OnIncreaseCenterY(object sender, EventArgs e)
        {
            if (m_currentSprite == null) return;
               
            AddCenterPos(m_currentSprite, 0, 1);
            DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            txtCenterY.Text = m_currentSprite.CenterPos.Y.ToString();
        }

        private void AddCenterPos(Sprite sprite, int x, int y)
        {
            sprite.CenterPos = new Point(sprite.CenterPos.X + x, sprite.CenterPos.Y + y);
        }

        private void OnUpdateAnimInfo(object sender, EventArgs e)
        {
            if (AnimTreeView.GetNodeCount(true) <= 0)
            {
                MessageBox.Show(@"There is no animation at all");
                return;
            }

            if (txtAnimName.Text.Length <= 0
             || txtDuration.Text.Length <= 0)
            {
                MessageBox.Show(@"Input animation name & duration first");
                return;
            }

            var  selectedNode = AnimTreeView.SelectedNode;
            bool result       = int.TryParse(selectedNode.Text, out int frameIndex);
            
            if (false == result || selectedNode.GetNodeCount(true) > 0)
            {
                int count = selectedNode.GetNodeCount(true);
                for (int i = 0; i < count; i++)
                {
                     selectedNode.Nodes[i].Name = txtAnimName.Text;
                }

                result = float.TryParse(txtDuration.Text, out float duration);
                var frames = m_mapAnimations[selectedNode.Text];

                if (result)
                {
                     foreach (var frame in frames)
                    {
                        frame.Duration = duration;
                    }
                }

                if (selectedNode.Text != txtAnimName.Text)
                {
                    m_mapAnimations.Remove(selectedNode.Text);
                    m_mapAnimations.Add(txtAnimName.Text, frames);
                }

                selectedNode.Text  = txtAnimName.Text;
                selectedNode.Name  = txtAnimName.Text;
                txtFrameIndex.Text = @"-";
            }
            else
            {
                if (false == m_mapAnimations.ContainsKey(txtAnimName.Text))
                {
                    MessageBox.Show(@"There is no animation with the name");
                    txtAnimName.Text   = @"";
                    txtFrameIndex.Text = @"-";
                    txtDuration.Text   = @"-";
                    return;
                }

                var frames              = m_mapAnimations[txtAnimName.Text];
                m_currentSprite          = frames[frameIndex];
                m_currentSprite.Duration = float.Parse(txtDuration.Text);
                txtDuration.Text        = m_currentSprite.Duration.ToString(CultureInfo.InvariantCulture);
                DisplayInfo();
            }
        }

        private void OnAddAnimation(object sender, EventArgs e)
        {
            //check empty
            if (txtAnimName.Text.Length <= 0
             || txtDuration.Text.Length <= 0)
            {
                MessageBox.Show(@"Input animation name & duration first");
                return;
            }

            //check valid duration 
            bool result = float.TryParse(txtDuration.Text, out float duration);
            if (false == result)
            {
                MessageBox.Show(@"Invalid input for duration");
                return;
            }

            if (false == m_mapAnimations.ContainsKey(txtAnimName.Text))
            {
                var node = new TreeNode(txtAnimName.Text)
                {
                    Name = txtAnimName.Text,
                };
                node.Nodes.Add(txtAnimName.Text, @"0");

                AnimTreeView.Nodes.Add(node);

                var sprites = new List<Sprite>
                {
                    m_currentSprite
                };
                m_mapAnimations[txtAnimName.Text] = sprites;
                m_currentSprite.Duration          = duration;
            }
            else
            {
                if (m_mapAnimations[txtAnimName.Text].Contains(m_currentSprite))
                {
                    MessageBox.Show(@"already exists");
                    return;
                }

                int frameIndex          = m_mapAnimations[txtAnimName.Text].Count;
                m_currentSprite.Duration = float.Parse(txtDuration.Text);
                m_mapAnimations[txtAnimName.Text].Add(m_currentSprite);
                var newNode = new TreeNode(frameIndex.ToString())
                {
                    Name = txtAnimName.Text,
                };

                var animNodes = AnimTreeView.Nodes.Find(txtAnimName.Text, true);
                animNodes.First().Nodes.Add(newNode);
            }
        }

        private void OnRemoveAnimation(object sender, EventArgs e)
        {
            if (AnimTreeView.GetNodeCount(true) <= 0)
                return;

            var  selectedNode = AnimTreeView.SelectedNode;
            bool result       = int.TryParse(selectedNode.Text, out int frameIndex);
            if (false == result || selectedNode.GetNodeCount(true) > 0)
            {
                m_mapAnimations[selectedNode.Text].Clear();
                m_mapAnimations.Remove(selectedNode.Text);
                selectedNode.Remove();
            }
            else
            {
                var parent = selectedNode.Parent;
                var frames = m_mapAnimations[parent.Text];
                frames.RemoveAt(frameIndex);

                var next = selectedNode.NextNode;
                while (null != next)
                {
                    next.Text = (int.Parse(next.Text) - 1).ToString();
                    next      = next.NextNode;
                }

                selectedNode.Remove();

                if (frames.Count == 0)
                {
                    m_mapAnimations.Remove(parent.Text);
                    parent.Remove();
                }
            }
        }

        private void OnExecuteAnimation(object sender, EventArgs e)
        {
            if (false == m_isAnimationStarted)
            {
                AnimTimer.Start();
                btnStart.Text       = @"정지";
                m_isAnimationStarted = true;
            }
            else
            {
                AnimTimer.Stop();
                btnStart.Text       = @"재생";
                m_isAnimationStarted = false;
            }
        }

        private void DrawAnimSprite(Bitmap bitmap, Point centerPos, int scale)
        {
            bitmap = DrawBitmap(bitmap, scale);

            var spriteRectangle = new Rectangle(0,0,bitmap.Width,bitmap.Height);
            var destRectangle   = new Rectangle(m_animBackBufferCenter.X - centerPos.X * scale,
                                                m_animBackBufferCenter.Y - centerPos.Y * scale,
                                                bitmap.Width,
                                                bitmap.Height);

            using (var g = Graphics.FromImage(m_animBackBuffer))
            {
                g.PixelOffsetMode   = PixelOffsetMode.HighQuality;
                g.InterpolationMode = InterpolationMode.NearestNeighbor;
                g.Clear(Color.Transparent);
                g.DrawImage(bitmap, destRectangle, spriteRectangle, GraphicsUnit.Pixel);
            }

            m_animBackBuffer = DrawCenterLine(m_animBackBuffer, m_animBackBufferCenter, scale);
            m_animBackBuffer.MakeTransparent(m_backGroundColor);
            animationBox.Image = m_animBackBuffer;
        }

        private void OnTreeViewClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            bool result = int.TryParse(e.Node.Text, out int frameIndex);
            if (false == result || e.Node.GetNodeCount(true) > 0)
            {
                txtAnimName.Text   = e.Node.Text;
                txtFrameIndex.Text = @"-";
                txtDuration.Text   = @"-";

                //Draw Animations with the first frame to the animation pictureBox
                m_currentAnimationFrames = m_mapAnimations[e.Node.Text];
                m_currentAnimSprite      = m_currentAnimationFrames.First();
                m_currentSprite          = m_currentAnimSprite;
                m_frameIndex             = 0;
                DrawAnimSprite(m_currentAnimSprite.Bitmap, m_currentAnimSprite.CenterPos, m_spriteScale);
            }
            else
            {
                m_currentSprite = m_mapAnimations[e.Node.Name][frameIndex];

                txtFrameIndex.Text = frameIndex.ToString();
                txtDuration.Text   = m_currentSprite.Duration.ToString(CultureInfo.InvariantCulture);
                DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
                DisplayInfo();
            }
        }

        private void AnimTreeView_ItemDrag(object sender, ItemDragEventArgs e)
        {
            DoDragDrop(e.Item, DragDropEffects.Move);
        }

        private void Reset()
        {
            m_frameIndex      = 0;
            m_accumulatedTime = 0;
            m_isFinished      = false;
        }

        private void Update(float deltaTime)
        {
            if (m_currentAnimSprite == null            
             || m_currentAnimationFrames.Count() == 1)
                return;

            if (m_isFinished)
            {
                 Reset();
            }

            m_accumulatedTime += deltaTime;

            if (m_accumulatedTime > m_currentAnimationFrames[m_frameIndex].Duration)
            {
                m_accumulatedTime -= m_currentAnimationFrames[m_frameIndex].Duration;
                ++m_frameIndex;

                if (m_frameIndex >= m_currentAnimationFrames.Count)
                {
                    m_frameIndex -= 1;
                    m_isFinished =  true;
                }
            }
        }

        private void Render()
        {
            if (m_currentAnimSprite == null             
             || m_currentAnimationFrames.Count() == 1)
                return;
            DrawAnimSprite(m_currentAnimationFrames[m_frameIndex].Bitmap,
                           m_currentAnimationFrames[m_frameIndex].CenterPos,
                           m_spriteScale);
        }

        private void TimerEvent(object sender, EventArgs e)
        {
            Update(m_deltaTime);
            Render();
        }

        private void SelectBackgroundColor(Point mousePos)
        {
            SelectColor(m_resizeSheetBitmap.GetPixel(mousePos.X, mousePos.Y));
        }

        private void SelectColor(Color color)
        {
            txtRed.Text   = color.R.ToString();
            txtGreen.Text = color.G.ToString();
            txtBlue.Text  = color.B.ToString();
            using (var g = backGroundColorBox.CreateGraphics())
            {
                g.FillRectangle(new SolidBrush(color),
                                new Rectangle(0, 0, backGroundColorBox.Width, backGroundColorBox.Height));
            }

            m_backGroundColor = color;
        }

        private void TxtRed_TextChanged(object sender, EventArgs e)
        {
            bool result = int.TryParse(txtRed.Text, out int color);
            if (result == false || color  <= 0) color = 0;
            if (color > 255) color = 255;
            SelectColor(Color.FromArgb(color, m_backGroundColor.G, m_backGroundColor.B));
        }

        private void TxtGreen_TextChanged(object sender, EventArgs e)
        {
            bool result = int.TryParse(txtGreen.Text, out int color);
            if (result == false || color  <= 0) color = 0;
            if (color > 255) color = 255;
            SelectColor(Color.FromArgb(m_backGroundColor.R, color, m_backGroundColor.B));
        }

        private void TxtBlue_TextChanged(object sender, EventArgs e)
        {
            bool result = int.TryParse(txtBlue.Text, out int color);
            if (result == false || color  <= 0) color = 0;
            if (color > 255) color = 255;
            SelectColor(Color.FromArgb(m_backGroundColor.R, m_backGroundColor.G, color));
        }

        private enum Tool
        {
            Arrow,
            ColorPick,
            Capture,
        }

        private void OnSave(object sender, EventArgs e)
        {
            SaveData();
        }

        private void OnExportToXML(object sender, EventArgs e)
        {
            if (m_mapAnimations.Count <= 0)
            {
                MessageBox.Show(@"There is no data to export ");
                return;
            }

            var saveFileDialog = new SaveFileDialog
            {
                Filter           = @"xml | *.xml",
                InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            };

            saveFileDialog.ShowDialog();
            if (string.Empty == saveFileDialog.FileName)
                return;

            var settings = new XmlWriterSettings
            {
                Indent             = true,
                IndentChars        = "\t",
                NewLineChars       = "\r\n",
                NewLineHandling    = NewLineHandling.Replace,
                OmitXmlDeclaration = false,
                Encoding           = Encoding.UTF8,
            };

            using (var writer = XmlWriter.Create(saveFileDialog.FileName, settings))
            {
                writer.WriteStartDocument();

                writer.WriteStartElement("TextureAtlas");
                writer.WriteAttributeString("imagePath", Path.GetFileName(m_spriteSheetName.Replace(".bmp", "")));
                writer.WriteAttributeString("width", m_originalSheetBitmap.Width.ToString());
                writer.WriteAttributeString("height", m_originalSheetBitmap.Height.ToString());
                writer.WriteAttributeString("R", m_backGroundColor.R.ToString());
                writer.WriteAttributeString("G", m_backGroundColor.G.ToString());
                writer.WriteAttributeString("B", m_backGroundColor.B.ToString());
                writer.WriteAttributeString("A", m_backGroundColor.A.ToString());

                foreach (var data in m_mapAnimations)
                {
                    foreach (var sprite in data.Value)
                    {
                        writer.WriteStartElement("sprite");
                        writer.WriteAttributeString("n", data.Key);
                        writer.WriteAttributeString("x", sprite.LeftTop.X.ToString());
                        writer.WriteAttributeString("y", sprite.LeftTop.Y.ToString());
                        writer.WriteAttributeString("w", sprite.Width.ToString());
                        writer.WriteAttributeString("h", sprite.Height.ToString());
                        writer.WriteAttributeString("centerX", sprite.CenterPos.X.ToString());
                        writer.WriteAttributeString("centerY", sprite.CenterPos.Y.ToString());
                        writer.WriteAttributeString("isFlipped", sprite.FlipImage.ToString());
                        writer.WriteAttributeString("duration", sprite.Duration.ToString());
                        writer.WriteEndElement();
                    }
                }
                writer.WriteEndElement();

                writer.WriteEndDocument();
                writer.Flush();
            }
        }

        private void OnClosing(object sender, FormClosingEventArgs e)
        {
            if (false == m_isSaved && m_mapAnimations.Count > 0)
            {
                var result = MessageBox.Show(@"Do you want to save?", string.Empty, MessageBoxButtons.YesNoCancel);
                if (DialogResult.Yes == result)
                    e.Cancel = !SaveData();
                else if (DialogResult.Cancel == result)
                    e.Cancel = true;
            }
        }

        private bool SaveData()
        {
            var saveFileDialog = new SaveFileDialog
            {
                Filter           = @"Data | *.anim",
                InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            };
            saveFileDialog.ShowDialog();
            if (string.Empty == saveFileDialog.FileName)
                return false;

            var fileStream = File.OpenWrite(saveFileDialog.FileName);
            using (var writer = new BinaryWriter(fileStream))
            {
                var fileInfo = new FileInfo(m_spriteSheetName);

                writer.Write(fileInfo.Name);
                writer.Write(m_backGroundColor.ToArgb());
                writer.Write(m_mapAnimations.Count);

                foreach (var data in m_mapAnimations)
                {
                    string animationName       = data.Key;
                    int    animationFrameCount = data.Value.Count;

                    writer.Write(animationName);
                    writer.Write(animationFrameCount);

                    foreach (var sprite in data.Value)
                    {
                        var   startPos  = new Point(sprite.LeftTop.X, sprite.LeftTop.Y);
                        int   width     = sprite.Width;
                        int   height    = sprite.Height;
                        var   centerPos = new Point(sprite.CenterPos.X, sprite.CenterPos.Y);
                        float duration  = sprite.Duration;
                        bool  isFlipped = sprite.FlipImage;

                        writer.Write(startPos.X);
                        writer.Write(startPos.Y);
                        writer.Write(width);
                        writer.Write(height);
                        writer.Write(centerPos.X);
                        writer.Write(centerPos.Y);
                        writer.Write(duration);
                        writer.Write(isFlipped);
                    }
                }
            }
            return true;
        }

        private void OnLoad(object sender, EventArgs e)
        {
            LoadData();
        }

        private void LoadData()
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter           = @"Data |*.anim",
                InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            };

            if (DialogResult.OK != openFileDialog.ShowDialog())
                return;

            using (var reader = new BinaryReader(File.Open(openFileDialog.FileName, FileMode.Open)))
            {
                m_spriteSheetName = reader.ReadString();
                if (false == File.Exists(m_spriteSheetName))
                {
                    MessageBox.Show($@"Sprite sheet Image : {m_spriteSheetName} not found!. you can't load the animation info without it",
                                    string.Empty, MessageBoxButtons.OK);
                    return;
                }

                m_backGroundColor = Color.FromArgb(reader.ReadInt32());

                m_originalSheetBitmap = new Bitmap(m_spriteSheetName);
                m_resizeSheetBitmap   = m_originalSheetBitmap;

                spriteSheetBox.Width  = m_originalSheetBitmap.Width;
                spriteSheetBox.Height = m_originalSheetBitmap.Height;
                spriteSheetBox.Image  = m_originalSheetBitmap;

                m_backBuffer       = new Bitmap(spriteBox.Width, spriteBox.Height);
                m_backBufferCenter = new Point(m_backBuffer.Width, m_backBuffer.Height);

                m_animBackBuffer       = new Bitmap(animationBox.Width, animationBox.Height);
                m_animBackBufferCenter = new Point(m_animBackBuffer.Width, m_animBackBuffer.Height);

                int animationCount = reader.ReadInt32();
                for (int i = 0; i < animationCount; ++i)
                {
                    string animationName       = reader.ReadString();
                    int    animationFrameCount = reader.ReadInt32();
                    var    sprites             = new List<Sprite>();

                    for (int j = 0; j < animationFrameCount; ++j)
                    {
                        var   startPos  = new Point(reader.ReadInt32(), reader.ReadInt32());
                        int   width     = reader.ReadInt32();
                        int   height    = reader.ReadInt32();
                        var   centerPos = new Point(reader.ReadInt32(), reader.ReadInt32());
                        float duration  = reader.ReadSingle();
                        bool  isFlipped = reader.ReadBoolean();

                        var bitmap = m_originalSheetBitmap.Clone(
                                                                new Rectangle(startPos, new Size(width, height)),
                                                                PixelFormat.Format32bppArgb);
                        if (isFlipped) 
                        { 
                            bitmap.RotateFlip(RotateFlipType.RotateNoneFlipX);
                        }
                        var sprite = new Sprite();
                        sprite.LoadData(bitmap, startPos, centerPos, width, height, duration, isFlipped);
                        sprites.Add(sprite);
                    }
                    m_mapAnimations.Add(animationName, sprites);
                }
                LoadToTreeView();
            }
        }

        private void LoadToTreeView()
        {
            if (m_mapAnimations.Count <= 0) return;
            Debug.Assert(m_mapAnimations.Count > 0, "m_mapAnimations.Count>0");

            foreach (var data in m_mapAnimations)
            {
                var node = new TreeNode(data.Key)
                {
                    Name = data.Key,
                };

                for (int i = 0; i < data.Value.Count; ++i)
                {
                    node.Nodes.Add(data.Key, i.ToString());
                }
                AnimTreeView.Nodes.Add(node);
            }
        }

        private void AnimTreeView_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            if (string.Empty == e.Label)
            {
                MessageBox.Show(@"이름은 빈 값일 수 없습니다");
                e.CancelEdit = true;
                return;
            }

            if (null != e.Node.Parent)
            {
                MessageBox.Show(@"자식 노드의 이름은 변경할 수 없습니다");
                e.CancelEdit = true;
            }
            else
            {
                if (null == e.Label) return;

                if (m_mapAnimations.ContainsKey(e.Label))
                {
                    MessageBox.Show(@"중복된 이름이 존재합니다");
                    e.CancelEdit = true;
                }
                else
                {
                    string oldName = e.Node.Text;
                    var    sprites = m_mapAnimations[oldName];
                    m_mapAnimations.Remove(oldName);

                    string newName = e.Label;
                    m_mapAnimations.Add(newName, sprites);

                    e.Node.Name     = newName;
                    var currentNode = e.Node.FirstNode;
                    while (null != currentNode)
                    {
                        currentNode.Name = newName;
                        currentNode      = currentNode.NextNode;
                    }
                }
            }
        }

        private void OnFlipImage(object sender, EventArgs e)
        {
            if (null != m_currentSprite)
            {
                m_currentSprite.FlipImage = !(m_currentSprite.FlipImage);
                m_currentSprite.Bitmap.RotateFlip(RotateFlipType.RotateNoneFlipX);
                DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            }
        }

        private void txtCenterY_TextChanged(object sender, EventArgs e)
        {
            if (int.TryParse(txtCenterY.Text, out int centerY))
            {
                m_currentSprite.CenterPos = new Point(m_currentSprite.CenterPos.X, centerY);
                DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            }
        }

        private void txtCenterX_TextChanged(object sender, EventArgs e)
        {
            if (int.TryParse(txtCenterX.Text, out int centerX))
            {
                m_currentSprite.CenterPos = new Point(centerX, m_currentSprite.CenterPos.Y);
                DrawSprite(m_currentSprite.Bitmap, m_currentSprite.CenterPos, m_spriteScale);
            }
        }
    }
}