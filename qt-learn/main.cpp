#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QIntValidator>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QPaintDevice>
#include <QPoint>
#include <QPushButton>
#include <QScreen>
#include <QTextEdit>
#include <QWidget>

class Label : public QLabel {
 public:
  explicit Label(QWidget* parent = nullptr) : QLabel(parent) {
    setAlignment(Qt::AlignCenter);  // label->setAlignment(Qt::AlignHCenter |
                                    // Qt::AlignVCenter);

    setStyleSheet(R"(
    QLabel {
      background-color: #333333;
      color: #66ccff;
      font-size: 60px;
      margin: 10px;
    }
    )");
  }
};

class MainWindow : public QMainWindow {
 public:
  explicit MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
    QScreen const* currentScreen = QApplication::screenAt(QCursor::pos());
    if (currentScreen == nullptr) {
      currentScreen = QApplication::primaryScreen();
    }
    QRect const screenGeom = currentScreen->geometry();

    setWindowTitle("Qt Calculator");

    // win.setFixedSize(400, 400); // win.setFixedSize(QSize(400, 400));
    // win.setBaseSize(6000, 6000);
    // win.setGeometry(0, 0, 6000, 6000);

    QRect winGeo{0, 0, 500, 300};
    // winGeo.moveTopLeft(screenGeom.topLeft() + QPoint(600, 600));
    winGeo.moveCenter(screenGeom.center());
    setGeometry(winGeo);

    addToolBar("this is the tool bar!");

    QWidget* centeralwidget = new QWidget;
    setCentralWidget(centeralwidget);

    QGridLayout* layout = new QGridLayout;
    centeralwidget->setLayout(layout);

    m_line1 = new QLineEdit;
    m_line1->setText("");
    auto validator = new QDoubleValidator;
    // validator->setRange(0, 1, -4);
    m_line1->setValidator(validator);
    layout->addWidget(m_line1, 0, 0, 1, 4);

    {
      QPushButton* button = new QPushButton;
      button->setText("+/-");
      layout->addWidget(button, 1, 1);
      connect(button, &QPushButton::clicked, this, &MainWindow::pressClear);
    }

    {
      QPushButton* button = new QPushButton;
      button->setText("C");
      layout->addWidget(button, 1, 0);
      connect(button, &QPushButton::clicked, this, &MainWindow::pressClear);
    }

    {
      QPushButton* button = new QPushButton;
      button->setText("CM");
      layout->addWidget(button, 1, 1);
      connect(button, &QPushButton::clicked, this,
              &MainWindow::pressMemoryClear);
    }

    {
      QPushButton* button = new QPushButton;
      button->setText("M+");
      layout->addWidget(button, 1, 2);
      connect(button, &QPushButton::clicked, this,
              &MainWindow::pressMemoryPlus);
    }

    {
      QPushButton* button = new QPushButton;
      button->setText("M-");
      layout->addWidget(button, 1, 3);
      connect(button, &QPushButton::clicked, this,
              &MainWindow::pressMemoryMinus);
    }

    {
      QPushButton* button = new QPushButton;
      button->setText("MR");
      layout->addWidget(button, 1, 4);
      connect(button, &QPushButton::clicked, this,
              &MainWindow::pressMemoryRead);
    }

    for (int i = 1; i <= 9; ++i) {
      QPushButton* button = new QPushButton;
      button->setText(QString::number(i));
      layout->addWidget(button, 2 + (i - 1) / 3, (i - 1) % 3);

      connect(button, &QPushButton::clicked, this,
              [this, i] { pressNumber(i); });
    }

    {
      QPushButton* button = new QPushButton;
      button->setText("0");
      layout->addWidget(button, 5, 1);
      connect(button, &QPushButton::clicked, this, [this] { pressNumber(0); });
    }

    {
      QPushButton* button = new QPushButton;
      button->setText("X");
      layout->addWidget(button, 5, 2);
      connect(button, &QPushButton::clicked, this, &MainWindow::pressBackspace);
    }

    {
      QPushButton* button = new QPushButton;
      button->setText(".");
      layout->addWidget(button, 5, 0);
      connect(button, &QPushButton::clicked, this, &MainWindow::pressDot);
    }

    QList<QPair<Operator, QString>> operators = {
        {Operator::Plus, "+"},
        {Operator::Minus, "-"},
        {Operator::Multiply, "*"},
        {Operator::Divide, "/"},
    };

    for (int i = 0; i < operators.size(); ++i) {
      Operator op = operators[i].first;
      QString name = operators[i].second;

      QPushButton* button = new QPushButton;
      button->setText(name);
      layout->addWidget(button, i + 2, 3);
      connect(button, &QPushButton::clicked, this,
              [this, op] { pressOperator(op); });
    }

    QPushButton* equalButton = new QPushButton;
    equalButton->setText("=");
    layout->addWidget(equalButton, 6, 0, 1, 4);
    connect(equalButton, &QPushButton::clicked, this, &MainWindow::pressEqual);
  }

 private:
  // bool ok;
  QLineEdit* m_line1;
  // QLineEdit* m_line2;
  // QLineEdit* m_lineResult;

  enum Operator {
    None,
    Plus,
    Minus,
    Multiply,
    Divide,
  };
  double m_lastResult = 0;
  Operator m_lastOperator = None;
  double m_memoryVal = 0;

  auto setLineText(QString text) -> void {
    int pos;
    if (m_line1->validator()->validate(text, pos) != QValidator::Invalid) {
      while (text.size() >= 2 && text[0] == '0' &&
             (text[1] >= '0' && text[1] <= '9')) {
        text.remove(0, 1);
      }
      // if (text.isEmpty()) {
      //   text = "0";
      // }

      m_line1->setText(text);
    }
  }

  auto pressClear() -> void { setLineText(""); }

  auto pressMemoryClear() -> void { m_memoryVal = 0; }

  auto pressMemoryPlus() -> void {
    double value = m_line1->text().toDouble();
    m_memoryVal += value;
  }

  auto pressMemoryMinus() -> void {
    double value = m_line1->text().toDouble();
    m_memoryVal -= value;
  }

  auto pressMemoryRead() -> void {
    setLineText(QString::number(m_memoryVal, 'g', 10));
  }

  auto pressSign() -> void {
    QString text = m_line1->text();
    if (!text.isEmpty()) {
      if (text.startsWith("-")) {
        text.remove(0, 1);
      } else {
        text = '-' + text;
      }
    }
    setLineText(text);
  }

  auto pressOperator(Operator op) -> void {
    double value = m_line1->text().toDouble();
    m_lastResult = value;
    m_lastOperator = op;
    m_line1->setText("");
  }

  auto pressNumber(int num) -> void {
    QString text = m_line1->text();
    text += QString::number(num);
    setLineText(text);
  }

  auto pressBackspace() -> void {
    QString text = m_line1->text();
    if (!text.isEmpty()) {
      text.remove(text.size() - 1, 1);
    }
    setLineText(text);
  }

  auto pressDot() -> void {
    QString text = m_line1->text();
    text += ".";
    setLineText(text);
  }

  auto pressEqual() -> void {
    double value1 = m_lastResult;
    double value2 = m_line1->text().toDouble();
    double result;

    switch (m_lastOperator) {
      case Operator::None:
        result = value2;
      case Operator::Plus:
        result = value1 + value2;
        break;
      case Operator::Minus:
        result = value1 - value2;
        break;
      case Operator::Multiply:
        result = value1 * value2;
        break;
      case Operator::Divide:
        result = value1 / value2;
        break;
    }

    m_lastOperator = None;

    setLineText(QString::number(result, 'g', 10));
    // qDebug() << result;
  }

  // auto cancel() -> void { qDebug() << "cancel clicked!"; }
};

auto main(int argc, char** argv) -> int {
  const QApplication app(argc, argv);

  MainWindow mainWin;

  // << '\n' is automatically added
  qDebug() << "mainWin is hidden: " << mainWin.isHidden();
  mainWin.show();
  qDebug() << "mainWin is hidden: " << mainWin.isHidden();

  return QCoreApplication::exec();  // return app.exec();
}
