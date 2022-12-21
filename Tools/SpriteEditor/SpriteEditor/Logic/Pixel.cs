using System.Drawing;

namespace WindowsFormsApp.Logic
{
    public class Pixel
    {
        public Point Position { get; }
        public Color Color    { get; }

        public override bool Equals(object obj)
        {
            if (!(obj is Pixel other))
                return false;

            return (other.Position == Position) && (other.Color == Color);
        }

        public override int GetHashCode()
        {
            return Position.GetHashCode();
        }

        public Pixel(Point position, Color color)
        {
            Position    = position;
            Color       = color;
        }
    }
}