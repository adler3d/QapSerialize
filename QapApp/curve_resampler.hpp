class CurveResampler {
public:
    // Вариант 1: Фиксированное количество точек
    static std::vector<vec2d> resampleWithFixedCount(
        const std::vector<vec2d>& input, 
        int targetPointCount) 
    {
        if (input.size() < 2 || targetPointCount < 2) {
            return input;
        }
        
        // Вычисляем кумулятивные длины сегментов
        std::vector<double> cumulativeLengths;
        cumulativeLengths.reserve(input.size());
        cumulativeLengths.push_back(0.0);
        
        double totalLength = 0.0;
        for (size_t i = 1; i < input.size(); ++i) {
            double segmentLength = (input[i] - input[i-1]).Mag();
            totalLength += segmentLength;
            cumulativeLengths.push_back(totalLength);
        }
        
        // Если кривая имеет нулевую длину, возвращаем исходные точки
        if (totalLength <= 1e-10) {
            return input;
        }
        
        // Создаем новые равномерно распределенные точки
        std::vector<vec2d> result;
        result.reserve(targetPointCount);
        
        // Первая точка сохраняется
        result.push_back(input[0]);
        
        // Вычисляем шаг между точками
        double step = totalLength / (targetPointCount - 1);
        
        for (int i = 1; i < targetPointCount - 1; ++i) {
            double targetDistance = i * step;
            
            // Находим сегмент, содержащий целевую дистанцию
            size_t segmentIndex = 0;
            while (segmentIndex < cumulativeLengths.size() - 1 && 
                   cumulativeLengths[segmentIndex + 1] < targetDistance) {
                ++segmentIndex;
            }
            
            if (segmentIndex >= input.size() - 1) {
                segmentIndex = input.size() - 2;
            }
            
            // Интерполируем точку внутри сегмента
            double segmentStart = cumulativeLengths[segmentIndex];
            double segmentEnd = cumulativeLengths[segmentIndex + 1];
            double segmentLength = segmentEnd - segmentStart;
            
            if (segmentLength <= 1e-10) {
                result.push_back(input[segmentIndex]);
                continue;
            }
            
            double t = (targetDistance - segmentStart) / segmentLength;
            t = std::max(0.0, std::min(1.0, t)); // Кламп к [0, 1]
            
            vec2d p1 = input[segmentIndex];
            vec2d p2 = input[segmentIndex + 1];
            vec2d interpolated = p1 + (p2 - p1) * t;
            
            result.push_back(interpolated);
        }
        
        // Последняя точка сохраняется
        result.push_back(input.back());
        
        return result;
    }
    
    // Вариант 2: Заданное расстояние между точками
    static std::vector<vec2d> resampleWithFixedSpacing(
        const std::vector<vec2d>& input, 
        double spacing) 
    {
        if (input.size() < 2 || spacing <= 1e-10) {
            return input;
        }
        
        std::vector<double> cumulativeLengths;
        cumulativeLengths.reserve(input.size());
        cumulativeLengths.push_back(0.0);
        
        double totalLength = 0.0;
        for (size_t i = 1; i < input.size(); ++i) {
            double segmentLength = (input[i] - input[i-1]).Mag();
            totalLength += segmentLength;
            cumulativeLengths.push_back(totalLength);
        }
        
        if (totalLength <= 1e-10) {
            return input;
        }
        
        // Вычисляем количество точек на основе spacing
        int estimatedPoints = static_cast<int>(std::ceil(totalLength / spacing)) + 1;
        std::vector<vec2d> result;
        result.reserve(estimatedPoints);
        
        result.push_back(input[0]);
        
        double currentDistance = spacing;
        while (currentDistance < totalLength - 1e-10) {
            // Находим сегмент
            size_t segmentIndex = 0;
            while (segmentIndex < cumulativeLengths.size() - 1 && 
                   cumulativeLengths[segmentIndex + 1] < currentDistance) {
                ++segmentIndex;
            }
            
            if (segmentIndex >= input.size() - 1) {
                break;
            }
            
            // Интерполируем
            double segmentStart = cumulativeLengths[segmentIndex];
            double segmentEnd = cumulativeLengths[segmentIndex + 1];
            double segmentLength = segmentEnd - segmentStart;
            
            if (segmentLength <= 1e-10) {
                result.push_back(input[segmentIndex]);
                currentDistance += spacing;
                continue;
            }
            
            double t = (currentDistance - segmentStart) / segmentLength;
            t = std::max(0.0, std::min(1.0, t));
            
            vec2d p1 = input[segmentIndex];
            vec2d p2 = input[segmentIndex + 1];
            vec2d interpolated = p1 + (p2 - p1) * t;
            
            result.push_back(interpolated);
            currentDistance += spacing;
        }
        
        // Добавляем последнюю точку, если она достаточно далеко от предыдущей
        if ((input.back() - result.back()).Mag() > spacing * 0.5) {
            result.push_back(input.back());
        }
        
        return result;
    }
    
    // Вариант 3: Комбинированный - можно выбрать либо количество, либо расстояние
    enum ResampleMode {
        BY_COUNT,
        BY_SPACING
    };
    
    static std::vector<vec2d> resampleCurve(
        const std::vector<vec2d>& input,
        ResampleMode mode,
        double value) // либо количество точек, либо расстояние
    {
        switch (mode) {
            case BY_COUNT:
                return resampleWithFixedCount(input, static_cast<int>(value));
            case BY_SPACING:
                return resampleWithFixedSpacing(input, value);
            default:
                return input;
        }
    }
    
    // Дополнительная функция: вычисление текущей длины кривой
    static double calculateTotalLength(const std::vector<vec2d>& curve) {
        double length = 0.0;
        for (size_t i = 1; i < curve.size(); ++i) {
            length += (curve[i] - curve[i - 1]).Mag();
        }
        return length;
    }
  static void DrawPolyLineDenseOptimized(
    QapDev&qDev,
    const vec2d& a,
    const vec2d& b,
    const vector<vec2d>& arr,
    QapColor bef,
    QapColor aft,
    double line_size,
    int k = 4,
    double pn = 8,
    double pulse_koef = 1.0,
    double time=0.0
  ) {
    if (k <= 0) k = 1;
  
    // Быстрая проверка
    if (arr.empty()) {
      // Простой случай
      vector<vec2d> temp = {a, b};
      qDev.DrawPolyLine(temp, line_size, false);
      return;
    }
  
    // Используем BatchScope
    QapDev::BatchScope Scope(qDev);
  
    // Собираем полный путь
    const int total_segments = (static_cast<int>(arr.size()) + 1) * k;
  
    // Предрасчет констант
    const double segment_step = 1.0 / total_segments;
    const double sub_step = 1.0 / k;
    const double fourPi = Pi * 4.0;
  
    // Оптимизация: расчитываем длины только если pulse_koef != 0
    double len = 0.0;
    if (pulse_koef != 0.0 || pn != 8.0) {
      // Вычисляем длину пути
      len = a.dist_to(arr[0]);
      for (size_t i = 0; i + 1 < arr.size(); i++) {
        len += arr[i].dist_to(arr[i + 1]);
      }
      len += arr.back().dist_to(b);
      pn = len * 4.0 * pulse_koef / 24.0;
    }
    const double Pi_pn = Pi * pn;
  
    // Сохраняем начальный индекс вершин
    const int vertex_start = qDev.VPos;
  
    // Проходим по всем сегментам
    for (int seg = 0; seg < total_segments; seg++) {
      // Определяем, между какими точками исходного пути текущий сегмент
      int path_seg = seg / k;
      int sub_seg = seg % k;
    
      vec2d p0, p1;
    
      if (path_seg == 0) {
        p0 = a;
        p1 = arr[0];
      } else if (path_seg <= static_cast<int>(arr.size())) {
        p0 = arr[path_seg - 1];
        p1 = (path_seg == static_cast<int>(arr.size())) ? b : arr[path_seg];
      } else {
        break;
      }
    
      // Линейная интерполяция без вызова Lerp
      double t0 = sub_seg * sub_step;
      double t1 = t0 + sub_step;
    
      // Вычисляем точки сегмента
      vec2d pa = p0 + (p1 - p0) * t0;
      vec2d pb = p0 + (p1 - p0) * t1;
    
      // Предрасчет параметров
      double global_t = seg * segment_step+time;
    
      // Оптимизация: проверяем, нужны ли вычисления пульсации
      double ls = line_size;
      if (pulse_koef != 0.0) {
        double pulse = sin(global_t * Pi_pn);
        // Оптимизированный Lerp: line_size + line_size * (1.0 + pulse) * 0.5
        //ls = line_size * (1.5 + pulse * 0.5);
        ls=line_size + Lerp(0.0, line_size, 1.0 * (1.0 + pulse));
      }
    
      // Вектор направления и нормаль
      vec2d dir = pb - pa;
      if (dir.SqrMag() < 1e-10) continue;
    
      vec2d n = dir.Ort().SetMag(ls * 0.5);
    
      // Создаем 4 вершины для обводки и 4 для основной линии
      int base_index = qDev.VPos;
    
      // Обводка (черная) - вершины 0-3
      vec2d n_outline = n * ((ls + 4.0) / ls);
      qDev.AddVertex(pa + n_outline, 0xff000000, 0.5f, 0.5f); // 0
      qDev.AddVertex(pb - n_outline, 0xff000000, 0.5f, 0.5f); // 1
      qDev.AddVertex(pa - n_outline, 0xff000000, 0.5f, 0.5f); // 2
      qDev.AddVertex(pb + n_outline, 0xff000000, 0.5f, 0.5f); // 3
    
      // Основная линия (цветная) - вершины 4-7
      QapColor seg_color = QapColor::Mix(bef, aft, global_t);
      qDev.AddVertex(pa + n, seg_color, 0.5f, 0.5f); // 4
      qDev.AddVertex(pb - n, seg_color, 0.5f, 0.5f); // 5
      qDev.AddVertex(pa - n, seg_color, 0.5f, 0.5f); // 6
      qDev.AddVertex(pb + n, seg_color, 0.5f, 0.5f); // 7
    
      // Обводка - 2 треугольника
      qDev.AddTris(base_index + 0, base_index + 1, base_index + 2);
      qDev.AddTris(base_index + 0, base_index + 1, base_index + 3);
    
      // Основная линия - 2 треугольника
      qDev.AddTris(base_index + 4, base_index + 5, base_index + 6);
      qDev.AddTris(base_index + 4, base_index + 5, base_index + 7);
    }
  }
};
/*
#include <vector>
#include <algorithm>
#include <cmath>
*/
struct t_canvas {
  vector<QapColor> mem;
  vec2i wh; // width, height
  
  void draw_trigon_with_msaa_on_edges(const vec2d& a, const vec2d& b, const vec2d& c, QapColor color) {
    // Находим bounding box треугольника
    int minX = max(0, (int)floor(std::min({a.x, b.x, c.x})));
    int maxX = min(wh.x - 1, (int)ceil(std::max({a.x, b.x, c.x})));
    int minY = max(0, (int)floor(std::min({a.y, b.y, c.y})));
    int maxY = min(wh.y - 1, (int)ceil(std::max({a.y, b.y, c.y})));
    
    // Предрасчёт коэффициентов барицентрических координат
    double area2 = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (abs(area2) < 1e-9) return; // Вырожденный треугольник
    
    // Для каждого пикселя в bounding box
    for (int y = minY; y <= maxY; y++) {
      for (int x = minX; x <= maxX; x++) {
        // Центр пикселя (для проверки полного покрытия)
        double cx = x + 0.5;
        double cy = y + 0.5;
        
        // Проверяем, находится ли центр пикселя внутри треугольника
        double alpha = ((b.x - cx) * (c.y - cy) - (c.x - cx) * (b.y - cy)) / area2;
        double beta  = ((c.x - cx) * (a.y - cy) - (a.x - cx) * (c.y - cy)) / area2;
        double gamma = 1.0 - alpha - beta;
        
        bool inside = (alpha >= 0 && beta >= 0 && gamma >= 0);
        
        if (inside&&0) {
          // Центр пикселя внутри треугольника - полное покрытие
          QapColor& dst = mem[x + y * wh.x];
          // Здесь можно реализовать blending, но для простоты пишем цвет
          dst.r = color.r;
          dst.g = color.g;
          dst.b = color.b;
          dst.a = 255;
        } else {
          // Проверяем, пересекает ли треугольник этот пиксель (для MSAA на границах)
          // Используем 4 подвыборки на пиксель (2x2)
          int samplesCovered = 0;
          const int samples = 4*4;
          /*vec2d subSampleOffsets[4] = {
            {0.25, 0.25}, {0.75, 0.25}, 
            {0.25, 0.75}, {0.75, 0.75}
          };*/
          vec2d subSampleOffsets[16];int p=0;
          for(int y=0;y<4;y++){
            for(int x=0;x<4;x++){
              subSampleOffsets[p++]=vec2d(x,y)*(1.0/4.0);
            }
          }
          for (int s = 0; s < samples; s++) {
            double sx = x + subSampleOffsets[s].x;
            double sy = y + subSampleOffsets[s].y;
            
            alpha = ((b.x - sx) * (c.y - sy) - (c.x - sx) * (b.y - sy)) / area2;
            beta  = ((c.x - sx) * (a.y - sy) - (a.x - sx) * (c.y - sy)) / area2;
            gamma = 1.0 - alpha - beta;
            
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
              samplesCovered++;
            }
          }
          
          if (samplesCovered > 0) {
            // Пиксель на границе - записываем с альфой пропорционально покрытию
            QapColor& dst = mem[x + y * wh.x];
            double coverage = samplesCovered / (double)samples;
            dst=color;
            // Смешиваем с существующим цветом (простое наложение)
            if(dst.a){
              dst.r = (unsigned char)(color.r * coverage + dst.r * (1.0 - coverage));
              dst.g = (unsigned char)(color.g * coverage + dst.g * (1.0 - coverage));
              dst.b = (unsigned char)(color.b * coverage + dst.b * (1.0 - coverage));
            }
            //dst.a = uchar(255*coverage);
            dst.a=(unsigned char)(255 * coverage + dst.a * (1.0 - coverage));
          }
        }
      }
    }
  }
};

struct t_pixel {
  QapColor color[4];
  bool covered[4];

  t_pixel(){
    for(int i=0;i<4;i++){
      covered[i]=false;
      color[i]=0x00ffffff;
    }
  }
};

static const vec2d MSAA4[4]={
  {0.375,0.125},
  {0.875,0.375},
  {0.125,0.625},
  {0.625,0.875}
};
struct t_canvas2 {
  vector<t_pixel> mem;
  vec2i wh;

  static inline bool isTopLeft(int dx,int dy){
    return (dy>0)||(dy==0 && dx<0);
  }

  static inline int edge(int x0,int y0,int x1,int y1,int x,int y){
    return (x-x0)*(y1-y0)-(y-y0)*(x1-x0);
  }
  void draw_trigon_with_msaa_on_edges(
    const vec2d& A,const vec2d& B,const vec2d& C,QapColor color)
  {
    vec2d a=A,b=B,c=C;

    // нормализуем winding
    double area=(b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
    if(area>0) std::swap(b,c);

    const int FP=9;
    const int SCALE=1<<FP;

    int ax=int(a.x*SCALE);
    int ay=int(a.y*SCALE);

    int bx=int(b.x*SCALE);
    int by=int(b.y*SCALE);

    int cx=int(c.x*SCALE);
    int cy=int(c.y*SCALE);

    int minx=min(ax,min(bx,cx))>>FP;
    int maxx=max(ax,max(bx,cx))>>FP;
    int miny=min(ay,min(by,cy))>>FP;
    int maxy=max(ay,max(by,cy))>>FP;

    minx=max(minx,0);
    miny=max(miny,0);
    maxx=min(maxx,wh.x-1);
    maxy=min(maxy,wh.y-1);

    int dx0=bx-cx; int dy0=by-cy;
    int dx1=cx-ax; int dy1=cy-ay;
    int dx2=ax-bx; int dy2=ay-by;

    int bias0=isTopLeft(dx0,dy0)?0:-1;
    int bias1=isTopLeft(dx1,dy1)?0:-1;
    int bias2=isTopLeft(dx2,dy2)?0:-1;

    for(int y=miny;y<=maxy;y++)
    for(int x=minx;x<=maxx;x++)
    {
      for(int s=0;s<4;s++)
      {
        int sx=int((x+MSAA4[s].x)*SCALE);
        int sy=int((y+MSAA4[s].y)*SCALE);

        int e0=edge(bx,by,cx,cy,sx,sy)+bias0;
        int e1=edge(cx,cy,ax,ay,sx,sy)+bias1;
        int e2=edge(ax,ay,bx,by,sx,sy)+bias2;

        if(e0>=0 && e1>=0 && e2>=0)
        {
          auto&pix=mem[y*wh.x+x];
          pix.covered[s]=true;
          pix.color[s]=color;
        }
      }
    }
  }
  void resolve_to_final(vector<QapColor>& out)
  {
    out.resize(mem.size());

    for(size_t i=0;i<mem.size();i++)
    {
      double r=0,g=0,b=0;
      int n=0;
      for(int s=0;s<4;s++)
      {
        auto&cell=mem[i];
        if(!cell.covered[s])continue;
        QapColor c=cell.color[s];
        r+=c.r;
        g+=c.g;
        b+=c.b;
        n++;
      }
      if(n){
        out[i].r=r/n;
        out[i].g=g/n;
        out[i].b=b/n;
        out[i].a=255*n/4;
      }else out[i]=0x00ffffff;
    }
  }
};
#include <vector>
#include <cmath>
#include <algorithm>

struct FastGaussianBlur {
  int radius;
  std::vector<float> kernel;
  std::vector<float> weights;  // предрасчитанные веса
  FastGaussianBlur(int r = 3, float sigma = -1.0f) : radius(r) {
    if (sigma <= 0) {
      sigma = radius / 3.0f;  // эвристика: охватывает ~99% энергии
    }
    
    int size = 2 * radius + 1;
    kernel.resize(size);
    weights.resize(size * size);
    
    // Предрасчёт 1D ядра
    float sum = 0;
    for (int i = -radius; i <= radius; i++) {
      float val = std::exp(-(i*i) / (2 * sigma * sigma));
      kernel[i + radius] = val;
      sum += val;
    }
    
    // Нормализация 1D ядра
    for (int i = 0; i < size; i++) {
      kernel[i] /= sum;
    }
    
    // Предрасчёт 2D матрицы весов (kernel[x] * kernel[y])
    for (int y = 0; y < size; y++) {
      for (int x = 0; x < size; x++) {
        weights[y * size + x] = kernel[x] * kernel[y];
      }
    }
  }
  
  // Применить блур к картинке (in-place или out-of-place)
  template<typename ColorType>
  void apply(const std::vector<ColorType>& input, 
         std::vector<ColorType>& output,
         int width, int height) {
    
    output.resize(input.size());
    int size = 2 * radius + 1;
    
    // Для каждого пикселя
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        float r = 0, g = 0, b = 0, a = 0;
        float totalWeight = 0;
        
        // Сбор соседей
        for (int ky = -radius; ky <= radius; ky++) {
          int ny = y + ky;
          if (ny < 0 || ny >= height) continue;
          
          for (int kx = -radius; kx <= radius; kx++) {
            int nx = x + kx;
            if (nx < 0 || nx >= width) continue;
            
            float weight = weights[(ky + radius) * size + (kx + radius)];
            
            const auto& c = input[nx + ny * width];
            r += c.r * weight;
            g += c.g * weight;
            b += c.b * weight;
            a += c.a * weight;
            totalWeight += weight;
          }
        }
        
        // Нормализация (на случай, если у границ вес меньше)
        if (totalWeight > 0) {
          auto& out = output[x + y * width];
          out.r = (unsigned char)(r / totalWeight);
          out.g = (unsigned char)(g / totalWeight);
          out.b = (unsigned char)(b / totalWeight);
          out.a = (unsigned char)(a / totalWeight);
        }
      }
    }
  }
  
  // Оптимизированная версия с раздельным проходом (быстрее)
  template<typename ColorType>
  void applySeparable(const std::vector<ColorType>& input,
            std::vector<ColorType>& output,
            int width, int height) {
    
    std::vector<ColorType> temp(input.size());
    int size = 2 * radius + 1;
    
    // Горизонтальный проход
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        float r = 0, g = 0, b = 0, a = 0;
        float totalWeight = 0;
        
        for (int kx = -radius; kx <= radius; kx++) {
          int nx = x + kx;
          if (nx < 0 || nx >= width) continue;
          
          float weight = kernel[kx + radius];
          const auto& c = input[nx + y * width];
          
          r += c.r * weight;
          g += c.g * weight;
          b += c.b * weight;
          a += c.a * weight;
          totalWeight += weight;
        }
        
        if (totalWeight > 0) {
          auto& out = temp[x + y * width];
          out.r = (unsigned char)(r / totalWeight);
          out.g = (unsigned char)(g / totalWeight);
          out.b = (unsigned char)(b / totalWeight);
          out.a = (unsigned char)(a / totalWeight);
        }
      }
    }
    
    // Вертикальный проход
    output.resize(input.size());
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        float r = 0, g = 0, b = 0, a = 0;
        float totalWeight = 0;
        
        for (int ky = -radius; ky <= radius; ky++) {
          int ny = y + ky;
          if (ny < 0 || ny >= height) continue;
          
          float weight = kernel[ky + radius];
          const auto& c = temp[x + ny * width];
          
          r += c.r * weight;
          g += c.g * weight;
          b += c.b * weight;
          a += c.a * weight;
          totalWeight += weight;
        }
        
        if (totalWeight > 0) {
          auto& out = output[x + y * width];
          out.r = (unsigned char)(r / totalWeight);
          out.g = (unsigned char)(g / totalWeight);
          out.b = (unsigned char)(b / totalWeight);
          out.a = (unsigned char)(a / totalWeight);
        }
      }
    }
  }
};