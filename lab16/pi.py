def chemotaxis(cells, search_space, chem_steps, swim_length, step_size,
    d_attr, w_attr, h_rep, w_rep)
  best = nil
  chem_steps.times do |j|
    moved_cells = []
    cells.each_with_index do |cell, i|
      sum_nutrients = 0.0
      evaluate(cell, cells, d_attr, w_attr, h_rep, w_rep)
      best = cell if best.nil? or cell[:cost] < best[:cost]
      sum_nutrients += cell[:fitness]
      swim_length.times do |m|
        new_cell = tumble_cell(search_space, cell, step_size)
        evaluate(new_cell, cells, d_attr, w_attr, h_rep, w_rep)
        best = cell if cell[:cost] < best[:cost]
        break if new_cell[:fitness] > cell[:fitness]
        cell = new_cell
        sum_nutrients += cell[:fitness]
      end
      cell[:sum_nutrients] = sum_nutrients
      moved_cells << cell
    end
    puts "  >> chemo=#{j}, f=#{best[:fitness]}, cost=#{best[:cost]}"
    cells = moved_cells
  end
  return [best, cells]
end
