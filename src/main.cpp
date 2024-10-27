#include "samarium/graphics/colors.hpp"
#include "samarium/samarium.hpp"

#include "SerialReader.hpp"

using namespace sm;
using namespace sm::literals;

// TODO crashes sometimes

auto main() -> i32
{
    auto bench     = Benchmark{};
    auto stopwatch = Stopwatch{};

    auto ser = SerialReader();
    bench.add("serial init");

    auto window = Window{{.dims = dims720}};
    bench.add("window init");


    auto plot = Plot("CascadiaCode.ttf");
    bench.add("plot init");

    const auto grid_dims = Dimensions{2, 2};
    auto rng             = RandomGenerator{};
    plot.traces["x"]     = {"#ff0f0f"_c, 0.008};
    plot.traces["y"]     = {"#05ff00"_c, 0.008};
    plot.traces["z"]     = {"#004dff"_c, 0.008};
    plot.title.text      = "Acceleration";

    auto frame_counter = 0;
    const auto draw    = [&]
    {
        ser.read();

        const auto boxes = subdivide_box(window.world_box(), grid_dims, 0.97);
        plot.box         = boxes[{0, 1}];
        // plot.add("x", Vector2{frame_counter / 1000.0,
        //                       noise::perlin_1d(frame_counter / 1000.0, {100.0}) - 0.5});
        // plot.add("y", Vector2{frame_counter / 1000.0,
        //                       noise::perlin_1d(frame_counter / 1000.0 + 100.0, {100.0}) - 0.5});
        // plot.add("z", Vector2{frame_counter / 1000.0,
        //                       noise::perlin_1d(frame_counter / 1000.0 + 200.0, {100.0}) - 0.5});

        // print("hello", ser.delta);

        for (const auto& line : ser.delta_parsed)
        {
            // plot.add(Vector2{std::stod(line.at(0)), std::stod(line.at(1))});
            // print(line);
            if (line[0] == "b'S'") { continue; }
            const auto time  = std::stod(line.at(1));
            const auto bno_x = std::stod(line.at(6));
            const auto bno_y = std::stod(line.at(7));
            const auto bno_z = std::stod(line.at(8));

            // print(time, )

            plot.add("x", Vector2{time, bno_x});
            plot.add("y", Vector2{time, bno_y});
            plot.add("z", Vector2{time, bno_z});
        }

        draw::background(colors::black);
        plot.draw(window);
        if (frame_counter == 0) { stopwatch.print(); }

        // window.pan();
        // window.zoom_to_cursor();
        frame_counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    };

    bench.add("remaining init");

    run(window, draw);
    bench.print();
}
