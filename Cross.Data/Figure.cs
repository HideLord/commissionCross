﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cross.Data
{
    public class Figure : BaseNotify, ICloneable
    {
        public List<Point> Points;

        public Figure()
        {
            Points = new List<Point>();
        }

        public object Clone()
        {
            var clone = new Figure();
            foreach(var p in Points)
            {
                clone.Points.Add(p);
            }
            return clone;
        }
    }
}
