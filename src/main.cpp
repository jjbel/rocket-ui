#include "rapidjson/document.h"

#include "samarium/graphics/colors.hpp"
#include "samarium/samarium.hpp"

#include "SerialReader.hpp"

using namespace sm;
using namespace sm::literals;

// TODO crashes sometimes
// TODO get format from json config file
// TODO add axes, markings

auto main() -> i32
{
    rapidjson::Document document;

    const auto json_config = file::read_unsafe(file::text, "format.json");

    if (document.Parse(json_config.c_str()).HasParseError())
    {
        print("Error: format.json invalid.");
        return 1;
    }

    auto bench     = Benchmark{};
    auto stopwatch = Stopwatch{};

    // clear existing data
    // system("del data.txt");
    const std::string empty_str = " ";
    std::ofstream("data.txt", std::ios::binary).write(empty_str.c_str(), empty_str.size());

    auto ser = SerialReader(document["port"].GetString());
    bench.add("serial init");

    auto window = Window{{.dims{1800, 960}}};
    bench.add("window init");

    // TODO should share the Text object
    // pass it as a reference
    auto plots = Map<std::string, Plot>{};
    bench.add("plots init");

    const auto grid_dims = Dimensions{static_cast<u64>(document["grid_dims"][0].GetInt()),
                                      static_cast<u64>(document["grid_dims"][1].GetInt())};

    for (rapidjson::SizeType i = 0; i < document["plots"].Size(); i++)
    {
        const auto& plot_json = document["plots"][i];
        auto& plot            = plots[plot_json["key"].GetString()];
        plot.title.text       = plot_json["title"].GetString();
        plot.title.scale      = 0.03F;

        for (rapidjson::SizeType i = 0; i < plot_json["traces"].Size(); i++)
        {
            const auto& trace = plot_json["traces"][i];
            plot.traces[trace["key"].GetString()] =
                Plot::Trace{Color{static_cast<u8>(trace["color"][0].GetInt()),
                                  static_cast<u8>(trace["color"][1].GetInt()),
                                  static_cast<u8>(trace["color"][2].GetInt())},
                            static_cast<f32>(trace["thickness"].GetFloat())};
            // TODO add hex parsing
        }
    }

    auto frame_counter = 0;

    const auto draw = [&]
    {
        bench.reset();
        ser.read();
        bench.add("serial read");
        // fmt::print("serial read ");

        const auto boxes       = subdivide_box(window.world_box(), grid_dims, 0.97);
        plots["acc"].box       = boxes[{0, 1}];
        plots["high"].box      = boxes[{0, 0}];
        plots["gyro"].box      = boxes[{2, 1}];
        plots["temp_pi"].box   = boxes[{1, 1}];
        plots["temp_data"].box = boxes[{1, 0}];
        plots["alt"].box       = boxes[{2, 0}];

        for (const auto& line : ser.delta_parsed)
        {
            if (line[1] == "b'S'") { continue; }
            if (line[1][0] == '(') { continue; } // csv header starts with (

            // TODO should catch error
            const auto time = std::stod(line.at(2));

            const auto plot_entry =
                [&](const std::string& plot_key, const std::string& trace_key, u64 index_in_line)
            {
                if (line.size() > index_in_line)
                {
                    const auto data = std::stod(line[index_in_line]);
                    plots[plot_key].add(trace_key, Vector2{time, data});
                }
            };

            plot_entry("acc", "x", 7);
            plot_entry("acc", "y", 8);
            plot_entry("acc", "z", 9);

            plot_entry("high", "x", 10);
            plot_entry("high", "y", 11);
            plot_entry("high", "z", 12);

            plot_entry("gyro", "x", 13);
            plot_entry("gyro", "y", 14);
            plot_entry("gyro", "z", 15);

            plot_entry("temp_pi", "default", 20);
            plot_entry("temp_data", "default", 4);
            plot_entry("alt", "default", 5);
        }
        bench.add("parse");
        // fmt::print("parse done\n");

        draw::background("#161b24"_c);
        bench.add("draw bg");

        for (auto& [key, plot] : plots) { plot.draw(window); }
        bench.add("draw plot");
        if (frame_counter == 0) { stopwatch.print(); }
        frame_counter++;
        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        bench.add_frame();
    };

    bench.add("remaining init");

    run(window, draw);
    bench.print();
}
