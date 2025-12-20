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
    double pulse_koef = 1.0
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
      double global_t = seg * segment_step;
    
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
