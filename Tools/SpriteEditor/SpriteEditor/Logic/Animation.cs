using System;
using System.Collections.Generic;
using System.Drawing;

namespace WindowsFormsApp.Logic
{
    public class Animation
    {
        public Animator     Animator        { get; set; }
        public string       Name            { get; set; }
        public List<Sprite> Frames          { get; set; }
        public int          FrameIndex      { get; set; }
        public float        AccumulatedTime { get; set; }
        public bool         IsFinished      { get; set; }

        public void Update(float deltaTime)
        {
            if (IsFinished) return;

            AccumulatedTime += deltaTime;

            if (AccumulatedTime > Frames[FrameIndex].Duration)
            {
                AccumulatedTime -= Frames[FrameIndex].Duration;
                ++FrameIndex;

                if (FrameIndex >= Frames.Count)
                {
                    FrameIndex -= 1;
                    IsFinished =  true;
                }
            }
        }

        public Bitmap Render()
        {
            return Frames[FrameIndex].Bitmap;
        }

        public void Create(string animationName, List<Sprite> frames)
        {
            Name   = animationName;
            Frames = frames;
        }

        public void Reset()
        {
            FrameIndex      = 0;
            AccumulatedTime = 0;
            IsFinished      = false;
        }
    }
}