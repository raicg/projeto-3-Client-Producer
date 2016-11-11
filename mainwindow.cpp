#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
    flag=0;
//  startTimer(100);
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  //linka o botão start com o slot putData
  connect(ui->pushButtonPut,
          SIGNAL(clicked(bool)),
          this,
          SLOT(start()));

  //linka o botão connect com o slot getIP
  connect(ui->pushButtonConnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(getIP()));

  //linka o botão disconnect com o slot disconnectS
  connect(ui->pushButtonDisconnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(disconnectS()));

  connect(ui->pushButtonStop,
          SIGNAL(clicked(bool)),
          this,
          SLOT(Stop()));


  connect(ui->horizontalSliderFreq,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(setVelocidade(int)));
}

// conecta o cliente ao servidor especificado
void MainWindow::tcpConnect(QString str){
  socket->connectToHost(str,1234);
  qDebug() << str;
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
  }
  else{
    qDebug() << "Disconnected";
  }
}

//copia o texto da caixa de texto para uma qstring
void MainWindow::getIP()
{
    QString str= ui->lineEditIP->text();
    tcpConnect(str);
}

void MainWindow::setVelocidade(int freq)
{
    this->frequencia = freq;
}

//desconecta o cliente do servidor
void MainWindow::disconnectS()
{
    socket->disconnectFromHost();
    qDebug() << "Disconnected";
}

void MainWindow::start()
{
    if (frequencia>0 && flag==0){
    timer = startTimer(1000/frequencia);
    flag=flag+1;
    }
}

void MainWindow::Stop()
{
    killTimer(timer);
    flag=flag-1;
}

//envia dados para o servidor com data/hora
void MainWindow::timerEvent(QTimerEvent *e)
{
        QDateTime datetime;
        QString str;
        datetime = QDateTime::currentDateTime();
        str = "set "+
                datetime.toString(Qt::ISODate)+
                " "+
                QString::number(qrand()%int(ui->lcdNumberMax->value()-ui->lcdNumberMin->value())+ui->lcdNumberMin->value());
        dataL.push_back(str);
        ui->plainTextEdit->appendPlainText(str);
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";

}

//destrutor da janela
MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}
