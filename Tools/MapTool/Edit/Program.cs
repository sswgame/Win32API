using System.Drawing;
using System.Drawing.Imaging;
using System.Security.Cryptography;
using System.Text;
using System.Xml;

namespace Edit;

public enum ParamType
{
    None,
    New,
    Exist,
    Concat,
    Slice
}

public class Program
{
    private readonly List<Bitmap?> _tilesInMapImage = new List<Bitmap?>();
    private readonly List<Bitmap?> _tilesInTileSet = new List<Bitmap?>();
    private readonly MD5           _md5             = MD5.Create();

    private readonly int     _size;
    public  Bitmap? TileBuffer { get; set; }
    public  Bitmap? MapBuffer  { get; set; }

    public List<string> HashesInTileSet  { get; } = new List<string>();
    public List<string> HashesInMapImage { get; } = new List<string>();

    public List<int>? Layer1Hashes { get; private set; }

    public static int TileWidth  { get; set; }
    public static int TileHeight { get; set; }

    public string Orientation { get; private set; } = string.Empty;
    public Bitmap? TileSetOutput { get; private set; }

    public static string? TileSetFilePath  { get; set; } = string.Empty;
    public static string? MapImageFilePath { get; set; } = string.Empty;

    public static string?[] TileSets   { get; set; }
    public static string?   OutputPath { get; set; }
    public static ParamType Parameter  { get; set; } = ParamType.None;

    public  Dictionary<string, Bitmap> UniqueTiles { get; } = new Dictionary<string, Bitmap>();

    private static void Main(string?[] args)
    {
        if (args.Length <= 0)
        {
            HelpMessage();
        }
        else
        {
            string? parameter = args[0];

            try
            {
                Console.WriteLine("Tile size is " + args[1] + " by " + args[2]);

                TileWidth =int.Parse(args[1]);
                TileHeight = int.Parse(args[2]);

                switch (parameter)
                {
                    case "-new":
                        MakeNewTileSet(args[3]);
                        break;
                    case "-make":
                        MakeWithExistingTileSet(args[3], args[4]);
                        break;
                    case "-concat":
                    {
                        string path = Path.Combine(Directory.GetCurrentDirectory(), args[3]);
                        if (File.GetAttributes(path).HasFlag(FileAttributes.Directory))
                            ConcatTileSets(Directory.GetFiles(path, "*.png").ToArray());
                        else
                            ConcatTileSets(args.Skip(3).ToArray());
                        break;
                    }
                    case "-slice":
                        SliceTileSet(args[3], args[4]);
                        break;
                    default:
                        HelpMessage();
                        break;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                throw;
            }

            var program = new Program();
            program.Execute();
        }
    }

    public static void HelpMessage()
    {
        Console.WriteLine(
                    @"Usage : 
                    >> Edit [parameter] [pixelWidth] [pixelHeight] [TileSet.png] [mapImage.png].
                    (parameters)
                    -new    : generate TileSet.
                    ex) Edit -new 16 16  mapImage.png
                    
                    -make   : make tile map with existing TileSet.
                    ex) Edit -make 16 16 TileSet.png mapImage.png

                    -concat : concat TileSets to produce an single tileSet With each of unique Tiles.
                    ex) Edit -concat 16 16 TileSet1.png TileSet2.png ... .

                    -slice  : slice TileSet with given pixelWidth & pixelHeight.
                    ex) Edit -slice 16 16 TileSet.png [outputFolderPath]."
                         );
    }

    public static void MakeNewTileSet(string? mapImage)
    {
        Parameter = ParamType.New;

        Console.WriteLine("Map Image file: " + mapImage);
        MapImageFilePath = mapImage;
    }
    public static void MakeWithExistingTileSet(string? tileSet, string? mapImage)
    {
        Parameter = ParamType.Exist;

        Console.WriteLine("Tile set file: " + tileSet);
        TileSetFilePath = tileSet;

        Console.WriteLine("Map Image file: " + mapImage);
        MapImageFilePath = mapImage;
    }
    public static void ConcatTileSets(string?[] tileSets)
    {
        Parameter = ParamType.Concat;

        Console.Write("Concatenating tileSets : ");
        TileSets = tileSets;

        foreach (string? tileSet in TileSets)
        {
            Console.Write(tileSet);
            Console.Write(@", ");
        }
        Console.WriteLine("\r\r");
    }
    public static void SliceTileSet(string? tileSet, string? outputPath)
    {
        Parameter       = ParamType.Slice;
        OutputPath      = outputPath;
        TileSetFilePath = tileSet;
        Console.WriteLine($"Slicing {TileSetFilePath} with (width:{TileWidth}, height:{TileHeight})");
    }

    private void Execute()
    {
        try
        {
            switch (Parameter)
            {
                case ParamType.None:
                    throw new Exception();
                case ParamType.New:
                {
                    //try to read map Image
                    MapBuffer = new Bitmap(Image.FromFile(MapImageFilePath));
                    Console.WriteLine("You have chosen to generate new tile set");
                    Console.WriteLine("Processing MapImageFilePath");

                    ProcessTiles(MapBuffer, HashesInMapImage, _tilesInMapImage);
                    GenerateTileSet();
                    
                    Compare();
                    Extract();
                }
                    break;
                case ParamType.Exist:
                {
                    MapBuffer     = new Bitmap(Image.FromFile(MapImageFilePath));
                    TileSetOutput = new Bitmap(Image.FromFile(TileSetFilePath));

                    Console.WriteLine("Processing TileSetOutput");
                    ProcessTiles(TileSetOutput, HashesInTileSet, _tilesInTileSet);

                    Console.WriteLine("Processing MapImageFilePath");
                    ProcessTiles(MapBuffer, HashesInMapImage, _tilesInMapImage);

                    Compare();
                    Extract();
                }
                    break;
                case ParamType.Concat:
                {
                    Bitmap[] buffers = new Bitmap[TileSets.Length];
                    for(int i=0;i<TileSets.Length;i++)
                    {
                        buffers[i] = new Bitmap(Image.FromFile(TileSets[i]));
                        Console.WriteLine($"Processing TileSet : {TileSets[i]}");
                        ProcessTiles(buffers[i], HashesInMapImage, _tilesInMapImage);
                    }
                    GenerateTileSet();
                }
                    break;
                case ParamType.Slice:
                {
                    MapBuffer = new Bitmap(Image.FromFile(TileSetFilePath));
                    Console.WriteLine($"Processing TileSet : {TileSetFilePath}");
                    ProcessTiles(MapBuffer, HashesInMapImage, _tilesInMapImage);

                    int    i    = 0;
                    string path = Path.Combine(Directory.GetCurrentDirectory(), OutputPath);
                    foreach (var bitmap in _tilesInMapImage)
                    {
                        bitmap.Save(Path.Combine(path, $"{i++}.png"), ImageFormat.Png);
                    }
                }
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
            
            Console.WriteLine("It is done!");
        }
        catch (IOException e)
        {
            Console.WriteLine(e.Message);
            Console.WriteLine(e.StackTrace);
        }
    }

    private void GenerateTileSet()
    {
        for (int i = 0; i < HashesInMapImage.Count; i++)
        {
            if (false == HashesInTileSet.Contains(HashesInMapImage[i]))
            {
                HashesInTileSet.Add(HashesInMapImage[i]);
            }
        }

        int localWidth  = 0;
        int localHeight = 0;
        int size        = (int)Math.Sqrt(UniqueTiles.Count);
        TileSetOutput   = new Bitmap(size * TileWidth, (size +2) * TileHeight);
        using( Graphics g = Graphics.FromImage(TileSetOutput))
        {
            foreach (KeyValuePair<string, Bitmap> data in UniqueTiles)
            {
                g.DrawImage(data.Value, localWidth, localHeight);
                localWidth += TileWidth;
                if (localWidth == TileSetOutput.Width)
                {
                    localHeight += TileHeight;
                    localWidth  =  0;
                }
            }
        }

        TileSetOutput.Save("newTileSet.png",ImageFormat.Png);
    }

    private void Extract()
    {
        Orientation = "orthogonal";

        var sb = new StringBuilder(MapImageFilePath);
        //Deleting file extension

        sb.Remove(sb.Length - 1, 1);
        sb.Remove(sb.Length - 1, 1);
        sb.Remove(sb.Length - 1, 1);
        sb.Append("tmx");

        string path = Path.Combine(Directory.GetCurrentDirectory(), sb.ToString());

        var setting = new XmlWriterSettings
        {
            Indent          = true,
            IndentChars     = "    ",
            NewLineChars    = "\r\n",
            NewLineHandling = NewLineHandling.Replace,
            Encoding        = new UTF8Encoding(false),
        };

        //generate tmx file
        var writer = XmlWriter.Create(path, setting);

        writer.WriteStartDocument();

        writer.WriteStartElement("map");
        writer.WriteAttributeString("version", "1.0");
        writer.WriteAttributeString("tiledversion", "1.7.2");
        writer.WriteAttributeString("orientation", Orientation);
        writer.WriteAttributeString("renderorder", "right-down");
        writer.WriteAttributeString("width", (MapBuffer.Width   / TileWidth).ToString());
        writer.WriteAttributeString("height", (MapBuffer.Height / TileHeight).ToString());
        writer.WriteAttributeString("tilewidth", TileWidth.ToString());
        writer.WriteAttributeString("tileheight", TileHeight.ToString());
        writer.WriteAttributeString("nextobjectid", "1");

        writer.WriteStartElement("tileset");
        writer.WriteAttributeString("firstgid", "1");
        writer.WriteAttributeString("source", "newTileSet.tsx");
        writer.WriteEndElement(); //end tag:tileset

        writer.WriteStartElement("layer");
        writer.WriteAttributeString("name", "Tile Layer 1");
        writer.WriteAttributeString("width", (MapBuffer.Width   / TileWidth).ToString());
        writer.WriteAttributeString("height", (MapBuffer.Height / TileHeight).ToString());

        writer.WriteStartElement("data");
        foreach (int i in Layer1Hashes)
        {
            writer.WriteStartElement("tile");
            writer.WriteAttributeString("gid", i.ToString());
            writer.WriteEndElement(); //end tag:tile
        }
        writer.WriteEndElement(); //end tag:data
        writer.WriteEndElement(); //end tag:layer
        writer.WriteEndElement(); //end tag:map

        writer.WriteEndDocument();

        writer.Close();

        //generate tsx file
        string fileName = @"newTileSet";
        path     = Path.Combine(Directory.GetCurrentDirectory(), $"{fileName}.tsx");
        
        writer = XmlWriter.Create(path, setting);

        writer.WriteStartDocument();

        writer.WriteStartElement("tileset");
        writer.WriteAttributeString("version", "1.5");
        writer.WriteAttributeString("name", fileName);
        writer.WriteAttributeString("tiledversion", "1.7.2");
        writer.WriteAttributeString("tilewidth", TileWidth.ToString());
        writer.WriteAttributeString("tileheight", TileHeight.ToString());
        writer.WriteAttributeString("tilecount", HashesInTileSet.Count.ToString());
        writer.WriteAttributeString("columns", _size.ToString());

        writer.WriteStartElement("image");
        writer.WriteAttributeString("source", $"{fileName}.png");
        writer.WriteAttributeString("width", TileSetOutput.Width.ToString());
        writer.WriteAttributeString("height", TileSetOutput.Height.ToString());
        writer.WriteEndElement(); //end tag:image

        writer.WriteEndElement(); //end tag:tileset

        writer.WriteEndDocument();

        writer.Close();
    }

    //compare 2 Lists with buffered images
    private void Compare()
    {
        int width = MapBuffer.Width / TileWidth;
        Layer1Hashes = new List<int>(HashesInMapImage.Count);

        for (int i = 0; i < HashesInMapImage.Count; i++)
        {
            Layer1Hashes.Add(-1);

            for (int j = 0; j < HashesInTileSet.Count; j++)
            {
                if (HashesInMapImage[i].Equals(HashesInTileSet[j]))
                {
                    Layer1Hashes[i] = j + 1;
                    break;
                }
            }
        }
        var missing = Layer1Hashes.FindAll(x => x == -1);
        Console.WriteLine($@"{missing.Count} tiles missing!");
    }

    private void ProcessTiles(Bitmap? bitmapInput, List<string> layer, List<Bitmap?> listBitmap)
    {
        TileBuffer = new Bitmap(TileWidth, TileHeight);

        int width  = bitmapInput.Width  / TileWidth;
        int height = bitmapInput.Height / TileWidth;

        for (int offsetY = 0; offsetY < height; offsetY++)
        {
            for (int offsetX = 0; offsetX < width; offsetX++)
            {
                //copy pixels per tile
                for (int y = offsetY * TileHeight; y < offsetY * TileHeight + TileHeight; y++)
                {
                    for (int x = offsetX * TileWidth; x < offsetX * TileWidth + TileWidth; x++)
                    {
                        TileBuffer.SetPixel(x - offsetX * TileWidth, y - offsetY * TileHeight,
                            bitmapInput.GetPixel(x, y));
                    }
                }
                GetHashForImage(TileBuffer, layer);
                listBitmap.Add(TileBuffer);
                TileBuffer = new Bitmap(TileWidth, TileHeight);
            }
        }
    }

    //register tile with unique hash value
    private void GetHashForImage(Bitmap? bitmap, List<string> layer)
    {
        try
        {
            var memoryStream = new MemoryStream();
            bitmap.Save(memoryStream, ImageFormat.Jpeg);

            byte[] imageInByte = memoryStream.ToArray();
            string str         = Encoding.UTF8.GetString(imageInByte);

            memoryStream.Close();

            string myHash = GetMd5Hash(_md5, str);

            //put hash in an array
            layer.Add(myHash);
            if (false == UniqueTiles.ContainsKey(myHash))
            {
                UniqueTiles.Add(myHash,bitmap);
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.StackTrace);
        }
    }

    //generate unique hash with md-5 algorithm
    private static string GetMd5Hash(MD5 md5Hash, string input)
    {
        // Convert the input string to a byte array and compute the hash.
        byte[] data = md5Hash.ComputeHash(Encoding.UTF8.GetBytes(input));

        // Create a new Stringbuilder to collect the bytes
        // and create a string.
        var sBuilder = new StringBuilder();

        // Loop through each byte of the hashed data
        // and format each one as a hexadecimal string.
        for (int i = 0; i < data.Length; i++) sBuilder.Append(data[i].ToString("x2"));

        // Return the hexadecimal string.
        return sBuilder.ToString();
    }
}