#include "samarium/graphics/colors.hpp"
#include "samarium/samarium.hpp"

#include <ranges>

using namespace sm;
using namespace sm::literals;

struct SerialReader
{
    static constexpr char python_file_contents[] =
#include "read_serial.py"
        ;

    const std::string python_file = "serial_read.py";
    const std::string data_file   = "data.txt";
    const char line_separator     = '\r';
    const char word_separator     = ',';

    std::string data{};
    std::string delta{};
    std::vector<std::vector<std::string>> delta_parsed{};
    u64 last_newline = 0;

    SerialReader()
    {
        std::ofstream(python_file, std::ios::binary)
            .write(python_file_contents, std::strlen(python_file_contents));

        // TODO let user enter this?
        start_reader();
    }

    void start_reader(std::string port = "COM5")
    {
        // https://stackoverflow.com/a/33903085
        system(fmt::format("start cmd /c \"python {} {}\"", python_file, port).c_str());
    }

    void read()
    {
        const auto new_data = file::read_unsafe(file::text, data_file);
        // cud be empty if write failed, or cud be same size if no change to file
        if (new_data.size() <= data.size()) { return; }

        u64 new_last_newline = 0;
        for (u64 i = last_newline + 1; i < new_data.size(); i++)
        {
            // TODO even with , end='\n', rpi still sends \r.
            // printing int(data[i]) says 13
            if (new_data[i] == line_separator) { new_last_newline = i; }
        }
        // print(last_newline, new_last_newline);
        delta        = new_data.substr(last_newline, new_last_newline);
        data         = new_data;
        last_newline = new_last_newline;
        parse_delta();
    }

    void parse_delta()
    {
        const auto to_str          = [](auto r) { return std::string(r.data(), r.size()); };
        const auto word_separator_ = this->word_separator;
        const auto line_to_vec     = [to_str, word_separator_](const auto& line)
        {
            // TODO will have to remove commas also, or split on ", "
            return to_str(line) | std::views::split(word_separator_) |
                   std::views::transform(to_str) | ranges::to<std::vector>();
        };

        // TODO filtering coz sometimes parsing the delta (but not when reading the existing file)
        // adds new lines like: [[], ["38439", "35.93917"], []]
        delta_parsed = delta | std::views::split(line_separator) |
                       std::views::transform(line_to_vec) |
                       std::views::filter([](const auto& line) { return !line.empty(); }) |
                       ranges::to<std::vector>();
    }

    // for debugging
    auto write() { std::ofstream("out.txt", std::ios::binary).write(data.c_str(), data.size()); }
    // TODO kill the python program in destructor
};

auto main() -> i32
{
    auto ser = SerialReader();
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    auto window          = Window{{.dims = dims720}};
    auto plot            = Plot("CascadiaCode.ttf");
    const auto grid_dims = Dimensions{2, 2};
    auto rng             = RandomGenerator{};
    plot.traces["x"]     = {"#ff0f0f"_c, 0.008};
    plot.traces["y"]     = {"#05ff00"_c, 0.008};
    plot.traces["z"]     = {"#004dff"_c, 0.008};
    plot.title.text      = "Acceleration";
    auto frame_counter   = 0;
    const auto draw      = [&]
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
        // window.pan();
        // window.zoom_to_cursor();
        frame_counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    };
    run(window, draw);
}
