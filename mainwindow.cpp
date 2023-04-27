#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QFileDialog"
#include "logic_op_2.h"
#include "QMessageBox"
#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_choose_file_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Choose file"),
                                                     "D:/Users/Alya/Documents/op_lab_2",
                                                     tr("Only CSV(*.csv)"));
    ui->lbl_file->setText(file_name);
    // �᫨ 䠩� �� ��࠭ - 㡨���, ��࠭ ����砥� ������ if (filname=="" && )
}


void MainWindow::on_btn_upload_clicked()
{
    string file_name =  ui->lbl_file->text().toStdString();
    int num_col_reg, len;
    int col_int=0;
    num_col_reg = read_headers(file_name, col_int);
    len = regions_to_combo_box(file_name, num_col_reg);
    region_data_put_on_table(file_name, col_int, len);
}

void MainWindow::region_data_put_on_table(string file_name, int col_int, int len){
    ifstream file(file_name);
    int i=0, j=0;
    string full, piece;
    getline(file, full);
    ui->table_file->setRowCount(len);
    ui->table_file->setColumnCount(col_int);
    while (getline(file, full)){
          //string *datas = (string *)malloc(col_int);
          stringstream ss(full);
          while (getline(ss, piece, ',')){
                ui->table_file->setItem(i, j, new QTableWidgetItem(QString::fromStdString(piece)));
                j++;
          }
          i++;
          j=0;
          //free(datas);
    }
}


int MainWindow::regions_to_combo_box(string file_name, int num_col_reg){
    ifstream file(file_name);
    QStringList regions;
    string full, reg;
    int len;
    int *z_arr = (int *)malloc(100);
    if (z_arr!=NULL){
        getline(file, full);
        while (getline(file, full)){
            int z_cnt=0;
            for (int i=0;i<full.length();i++){
                if (full[i]==','){
                   z_arr[z_cnt]=i;
                   z_cnt++;
                }
            }
            reg=full.substr(z_arr[num_col_reg-1]+1, z_arr[num_col_reg]-z_arr[num_col_reg-1]-1);
            regions.append(QString::fromStdString(reg));
        }
        len = regions.length();
        regions.removeDuplicates();
        regions.sort();
        ui->cmb_region->addItems(regions);
    } //else
        //��� �訡��
    free(z_arr);
    return len;
};


int MainWindow::read_headers(string file_name, int &col_int){
    int num_col_reg;
    ifstream file(file_name);
    QStringList headers;
    string header_str, h;
    getline(file, header_str);
    stringstream ss(header_str);
    while (getline(ss, h, ',')){
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(QString::fromStdString(h));
        ui->table_file->setItem(0, col_int, item);
        headers.append(QString::fromStdString(h));
        if (h == "region"){
            num_col_reg=col_int;
        }else{
            ui->cmb_columns->addItem(QString::fromStdString(h));
        }
        col_int++;
    }
    ui->table_file->setRowCount(1);
    ui->table_file->setColumnCount(col_int);
    ui->table_file->setHorizontalHeaderLabels(headers);
    return num_col_reg;
}


void MainWindow::on_btn_calc_clicked()
{
    logic base;
    base.file_name =  ui->lbl_file->text().toStdString();
    base.region = (ui->cmb_region->currentText()).toStdString();
    //data_region_to_table(base);
}

