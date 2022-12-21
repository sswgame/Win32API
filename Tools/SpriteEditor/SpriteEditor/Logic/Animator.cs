using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;

namespace WindowsFormsApp.Logic
{
    public class Animator
    {
        public Dictionary<string, Animation> MapAnimations { get; set; } = new Dictionary<string, Animation>();
        public Animation                     Current       { get; set; }
        public bool                          IsRepeat      { get; set; }

        public void Update(float deltaTime)
        {
            Current?.Update(deltaTime);

            if (IsRepeat && Current.IsFinished)
                Current?.Reset();
        }

        public Bitmap Render()
        {
            return Current?.Render();
        }

        public void LoadAnimation()
        {
            var animation = new Animation
            {
                Animator = this
            };
            MapAnimations.Add(animation.Name, animation);
        }

        public void CreateAnimation(string animationName, Dictionary<string, List<Sprite>> mapSprites)
        {
            var animation = FindAnimation(animationName);
            Debug.Assert(animation != null, "Animation already Exists");

            animation = new Animation
            {
                Animator = this
            };
            animation.Create(animationName, mapSprites[animationName]);

            MapAnimations.Add(animationName, animation);
        }

        private Animation FindAnimation(string animationName)
        {
            return (MapAnimations.ContainsKey(animationName)) ? MapAnimations[animationName] : null;
        }
    }
}