#include <QApplication>
#include <QCommandLineParser>
#include <QMessageBox>
#include <QTime>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <format>
#include <memory>
#include <numbers>
#include <print>
#include <random>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

class MainWindow;

class MemoryBox : public QGraphicsRectItem {
    static constexpr auto active_color   = Qt::red;
    static constexpr auto inactive_color = Qt::green;

    MainWindow* window;

  public:
    MemoryBox(const qreal ax,
              const qreal ay,
              const qreal wx,
              const qreal wy,
              QGraphicsScene* scene,
              MainWindow* w)
        : QGraphicsRectItem(ax, ay, wx, wy),
          window{ w } {
        setPen(QPen(QBrush(Qt::black), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        set_inactive();
        setRect(ax, ay, wx, wy);
        setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        scene->addItem(this);
    }

    void set_active() { setBrush(active_color); }

    void set_inactive() { setBrush(inactive_color); }

  protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* _);
};

class MainWindow : public QMainWindow {
    std::unique_ptr<QWidget> baseWidget{ std::make_unique<QWidget>(this) };
    std::unique_ptr<QGridLayout> baseWidgetGrid;
    std::unique_ptr<QGraphicsScene> graphicsScene;
    std::unique_ptr<QGraphicsView> graphicsView;

    std::vector<std::unique_ptr<MemoryBox>> boxes{};
    std::unordered_set<std::size_t> checked_boxes{};
    std::size_t next_box;

    using seed_t = std::random_device::result_type;
    std::mt19937_64 gen;

    std::size_t width  = 300uz;
    std::size_t height = 300uz;

    std::size_t box_width  = 20uz;
    std::size_t box_height = 20uz;

  public:
    MainWindow(const double hole_coeff, const seed_t seed, QWidget* parent = nullptr)
        : QMainWindow(parent),
          graphicsScene{ std::make_unique<QGraphicsScene>() },
          gen{ seed } {
        std::println("Game started!");
        std::println("box density: {}", hole_coeff);
        std::println("seed: {}", seed);
        struct P {
            double x, y;
            constexpr P rotated(const double rad) const {
                return { x * std::cos(rad) - y * std::sin(rad),
                         x * std::sin(rad) + y * std::cos(rad) };
            }
            constexpr P operator+(const P rhs) const { return { x + rhs.x, y + rhs.y }; }
        };
        auto top_left_quarter_coords = std::vector<P>{};

        auto distribution = std::uniform_real_distribution<double>(0., 1.);

        if (hole_coeff < 0. or hole_coeff > 1) {
            throw std::runtime_error{ std::format("Hole coefficient {} has to be in [0, 1]!",
                                                  hole_coeff) };
        }
        for (auto x = 0.; x < width / 2.; x += box_width) {
            for (auto y = 0.; y < height / 2.; y += box_height) {
                if (distribution(gen) < hole_coeff) top_left_quarter_coords.push_back({ x, y });
            }
        }

        std::ranges::for_each(top_left_quarter_coords, [&](const auto xy) {
            // Top left
            boxes.push_back(std::make_unique<MemoryBox>(xy.x,
                                                        xy.y,
                                                        box_width,
                                                        box_height,
                                                        graphicsScene.get(),
                                                        this));

            // Top right
            const auto xy_top_right =
                xy.rotated(std::numbers::pi_v<double> / 2.) + P{ static_cast<double>(width), 0. };
            boxes.push_back(std::make_unique<MemoryBox>(xy_top_right.x,
                                                        xy_top_right.y,
                                                        box_width,
                                                        box_height,
                                                        graphicsScene.get(),
                                                        this));

            // Bottom right
            const auto xy_bottom_right =
                xy.rotated(std::numbers::pi_v<double>)
                + P{ static_cast<double>(width), static_cast<double>(height) };

            boxes.push_back(std::make_unique<MemoryBox>(xy_bottom_right.x,
                                                        xy_bottom_right.y,
                                                        box_width,
                                                        box_height,
                                                        graphicsScene.get(),
                                                        this));

            // Bottom left
            const auto xy_bottom_left = xy.rotated(3. * std::numbers::pi_v<double> / 2.)
                                        + P{ 0., static_cast<double>(height) };

            boxes.push_back(std::make_unique<MemoryBox>(xy_bottom_left.x,
                                                        xy_bottom_left.y,
                                                        box_width,
                                                        box_height,
                                                        graphicsScene.get(),
                                                        this));
        });

        reset();

        baseWidgetGrid = std::make_unique<QGridLayout>(baseWidget.get());
        graphicsView   = std::make_unique<QGraphicsView>(baseWidget.get());
        graphicsView->setScene(graphicsScene.get());

        baseWidgetGrid->addWidget(graphicsView.get(), 0, 0, 1, 1);

        constexpr auto win_coeff = 1.3;
        this->setFixedSize(QSize(win_coeff * width, win_coeff * height));
        setCentralWidget(baseWidget.get());
    }

    void reset() {
        checked_boxes.clear();
        new_next_box();
        deactivate_all();
        boxes[next_box]->set_active();
    }

    void activate_checked() {
        for (const auto i : checked_boxes) { boxes[i]->set_active(); }
    }

    void deactivate_all() {
        for (auto& box : boxes) { box->set_inactive(); }
    }

    void activate_all() {
        for (auto& box : boxes) { box->set_active(); }
    }

    std::size_t get_index_of_box(const MemoryBox* box) {
        for (auto i = 0uz; i < boxes.size(); ++i) {
            if (boxes[i].get() == box) { return i; }
        }
        throw std::runtime_error{ "Invalid box!" };
    }

    void new_next_box() {
        const auto possible_indecies =
            std::views::iota(0uz, boxes.size())
            | std::views::filter([&](const auto i) { return not checked_boxes.contains(i); })
            | std::ranges::to<std::vector>();

        std::ranges::sample(possible_indecies, &next_box, 1, gen);
    }

    void take_step(const MemoryBox* box) {
        activate_all();

        auto stop{ false };

        const auto activated_box = get_index_of_box(box);
        if (next_box != activated_box) {
            std::println("You lose!");
            boxes[next_box]->setBrush(Qt::yellow);
            stop = true;
        } else {
            checked_boxes.insert(activated_box);
        }

        if (checked_boxes.size() == boxes.size()) {
            std::println("You win!");
            stop = true;
        }

        QTime dieTime = QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 100);

        if (stop) {
            std::println("Score: {}/{}", checked_boxes.size(), boxes.size());
            QCoreApplication::exit();
        } else {
            deactivate_all();
            activate_checked();
            new_next_box();
            boxes[next_box]->set_active();
        }
    }
};

void MemoryBox::mousePressEvent(QGraphicsSceneMouseEvent* _) { window->take_step(this); }

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    const auto c = [] {
        const auto s = std::getenv("MEMORY_GAME_BOX_DENSITY");
        if (s) return std::stod(s);
        else
            return 0.1;
    }();

    const auto seed = [] {
        const auto s = std::getenv("MEMORY_GAME_SEED");
        if (s) return static_cast<std::random_device::result_type>(std::stoul(s));
        else
            return std::random_device{}();
    }();

    MainWindow MainWin(c, seed);
    MainWin.show();
    return app.exec();
}
