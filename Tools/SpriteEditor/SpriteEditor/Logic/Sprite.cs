using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;

namespace WindowsFormsApp.Logic
{
    public class Sprite
    {
        public Bitmap   Bitmap      { get; private set; }

        public Point    LeftTop     { get; private set; }
        public Point    RightBottom { get; private set; }

        public int      Width       { get; private set; }
        public int      Height      { get; private set; }

        public bool     FlipImage   { get; set; }
        public Point    CenterPos   { get; set; }
        public float    Duration    { get; set; }

        public bool FindWithDFS(Bitmap bitmap, 
                                Point  mousePos, 
                                Color  backGroundColor)
        {
            if (IsBackGroundColor(bitmap.GetPixel(mousePos.X, mousePos.Y), backGroundColor))
                return false;

            //used for pixel boundary
            Rectangle rect   = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            var pixel        = new Pixel(mousePos, bitmap.GetPixel(mousePos.X, mousePos.Y));
            int quarterSize  = (bitmap.Width * bitmap.Height) / 4;

            //used for track pixel & DFS
            var setPixels    = new HashSet<Pixel>();
            var stackPixel   = new Stack<Pixel>();
            stackPixel.Push(pixel);
            setPixels.Add(pixel);

            while ( 0 < stackPixel.Count && stackPixel.Count < quarterSize)
            {
                var currentPixel = stackPixel.Pop();
                var position     = currentPixel.Position;

                var checkPoint   = new Point[4]
                {
                    new Point(position.X  , position.Y-1)
                   ,new Point(position.X-1, position.Y  )
                   ,new Point(position.X  , position.Y+1)
                   ,new Point(position.X+1, position.Y  )
                };

                foreach(var newPosition in checkPoint)
                {
                    if(rect.Contains(newPosition) 
                       && false == IsBackGroundColor(bitmap.GetPixel(newPosition.X,newPosition.Y),backGroundColor))
                    {
                        var neighbor = new Pixel(newPosition, bitmap.GetPixel(newPosition.X,newPosition.Y));
                        if (false == setPixels.Contains(neighbor))
                        {
                            setPixels.Add(currentPixel);
                            stackPixel.Push(neighbor);
                        }
                    }
                }
            }

            LeftTop     = new Point(setPixels.Min(p => p.Position.X), setPixels.Min(p => p.Position.Y));
            RightBottom = new Point(setPixels.Max(p => p.Position.X), setPixels.Max(p => p.Position.Y));
            Width       = RightBottom.X + 1 - LeftTop.X;
            Height      = RightBottom.Y + 1 - LeftTop.Y;
            CenterPos   = new Point(Width / 2, Height / 2);
            Bitmap      = bitmap.Clone(new Rectangle(LeftTop.X, LeftTop.Y, Width, Height), PixelFormat.Format32bppRgb);
            FlipImage   = false;

            return true;
        }

        public bool FindWithRange(Bitmap bitmap,
                                  Point  leftTop, 
                                  Point  rightBottom, 
                                  Color  backGroundColor)
        {
            //what we have to find
            int minX = leftTop.X;
            int minY = leftTop.Y;
            int maxX = rightBottom.X;
            int maxY = rightBottom.Y;

            bool isMinX = false;
            bool isMaxX = false;
            bool isMinY = false;
            bool isMaxY = false;

            //minX 구하기
            for (int i = leftTop.X; i <= rightBottom.X; i++)
            {
                for (int j = leftTop.Y; j <= rightBottom.Y; j++)
                {
                    if (IsBackGroundColor(bitmap.GetPixel(i, j), backGroundColor)) continue;

                    minX    = i;
                    isMinX  = true;
                    break;
                }

                if (isMinX) break;
            }

            //minY 구하기
            for (int j = leftTop.Y; j <= rightBottom.Y; j++)
            {
                for (int i = minX; i <= rightBottom.X; i++)
                {
                    if (IsBackGroundColor(bitmap.GetPixel(i, j), backGroundColor)) continue;

                    minY    = j;
                    isMinY  = true;
                    break;
                }

                if (isMinY) break;
            }

            //maxX 구하기
            for (int i = rightBottom.X; i >= minX; i--)
            {
                for (int j = minY; j <= rightBottom.Y; j++)
                {
                    if (IsBackGroundColor(bitmap.GetPixel(i, j), backGroundColor)) continue;

                    maxX    = i;
                    isMaxX  = true;
                    break;
                }

                if (isMaxX) break;
            }

            //maxY 구하기
            for (int j = rightBottom.Y; j >= minY; j--)
            {
                for (int i = minX; i <= maxX; i++)
                {
                    if (IsBackGroundColor(bitmap.GetPixel(i, j), backGroundColor)) continue;
                    maxY    = j;
                    isMaxY  = true;
                    break;
                }

                if (isMaxY) break;
            }

            bool anyFound = isMinX & isMinY & isMaxX & isMaxY;
            if (false == anyFound)
                return false;

            LeftTop     = new Point(minX, minY);
            RightBottom = new Point(maxX, maxY);
            Width       = RightBottom.X + 1 - LeftTop.X;
            Height      = RightBottom.Y + 1 - LeftTop.Y;
            CenterPos   = new Point(Width / 2, Height / 2);
            Bitmap      = bitmap.Clone(new Rectangle(LeftTop.X, LeftTop.Y, Width, Height), PixelFormat.Format32bppArgb);
            FlipImage   = false;

            return true;
        }

        private bool IsBackGroundColor(Color currentPixel, Color backGroundColor)
        {
            return currentPixel == backGroundColor;
        }
        public void LoadData(Bitmap bitmap, 
                             Point  startPos, 
                             Point  centerPos, 
                             int    width, 
                             int    height, 
                             float  duration,
                             bool   flipImage)
        {
            Bitmap      = bitmap;
            LeftTop     = startPos;
            CenterPos   = centerPos;
            Width       = width;
            Height      = height;
            Duration    = duration;
            FlipImage   = flipImage;
        }
    }
}