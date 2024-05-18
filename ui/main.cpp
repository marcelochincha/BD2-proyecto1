#include <QApplication>
#include <QHeaderView>
#include <QIcon>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <filesystem>
#include <iostream>

#include "../src/csvLoader.hpp"
#include "../src/database.hpp"

int main(int argc, char *argv[]) {
    // ------------------------------------------------------------------------

    std::cout << std::filesystem::current_path() << std::endl;

    Database *db = new Database();

    // ------------------------------------------------------------------------
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("DBII Proy1");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Top layout for help button
    QHBoxLayout *topLayout = new QHBoxLayout;

    // Spacer to push the help button to the right
    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topLayout->addItem(spacer);

    // ------------------------------------------------------------------------
    // Help button
    QPushButton *helpButton = new QPushButton;
    helpButton->setText("?");
    helpButton->setFixedSize(30, 30);
    topLayout->addWidget(helpButton);
    // ------------------------------------------------------------------------

    mainLayout->addLayout(topLayout);

    QLineEdit *queryInput = new QLineEdit;
    queryInput->setPlaceholderText("Enter SQL query...");
    mainLayout->addWidget(queryInput);

    QPushButton *executeButton = new QPushButton("Execute");
    mainLayout->addWidget(executeButton);

    QTableWidget *resultTable = new QTableWidget;
    resultTable->setColumnCount(10);  // Set the number of columns based on your needs
    resultTable->setHorizontalHeaderLabels({"CustomerID", "ProductID", "Quantity", "Price", "TransactionDate",
                                            "PaymentMethod", "StoreLocation", "ProductCategory", "DiscountApplied",
                                            "TotalAmount"});
    mainLayout->addWidget(resultTable);

    auto executeQuery = [=]() {
        resultTable->clearContents();
        // Print the query input to the console
        QString query = queryInput->text();
        std::cout << "Query: " << query.toStdString() << std::endl;
        std::cout << db->execute(query.toStdString()) << std::endl;

        resultTable->setRowCount(db->last_results.size());
        for (int i = 0; i < db->last_results.size(); i++) {
            Register reg = db->last_results[i];
            resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(reg.CustomerID)));
            resultTable->setItem(i, 1, new QTableWidgetItem(QString(reg.ProductID)));
            resultTable->setItem(i, 2, new QTableWidgetItem(QString::number(reg.Quantity)));
            resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(reg.Price)));
            resultTable->setItem(i, 4, new QTableWidgetItem(QString(reg.TransactionDate)));
            resultTable->setItem(i, 5, new QTableWidgetItem(QString(reg.PaymentMethod)));
            resultTable->setItem(i, 6, new QTableWidgetItem(QString(reg.StoreLocation)));
            resultTable->setItem(i, 7, new QTableWidgetItem(QString(reg.ProductCategory)));
            resultTable->setItem(i, 8, new QTableWidgetItem(QString::number(reg.DiscountApplied)));
            resultTable->setItem(i, 9, new QTableWidgetItem(QString::number(reg.TotalAmount)));
        }

        // Dummy data to simulate query result
        // resultTable->setRowCount(1);
        // resultTable->setItem(0, 0, new QTableWidgetItem("Data 1"));
        // resultTable->setItem(0, 1, new QTableWidgetItem("Data 2"));
        // resultTable->setItem(0, 2, new QTableWidgetItem("Data 3"));
    };

    QObject::connect(executeButton, &QPushButton::clicked, executeQuery);
    QObject::connect(queryInput, &QLineEdit::returnPressed, executeQuery);

    // ------------------------------------------------------------------------
    QObject::connect(helpButton, &QPushButton::clicked, [&]() {
        QMessageBox::information(&window, "Help",
                                 "Create table:\n"
                                 "create table Customer from file “C:\\data.csv” using index "
                                 "hash(“DNI”)\n\n"
                                 "Select:\n"
                                 "select * from Customer where DNI = x\n"
                                 "select * from Customer where DNI between x and y\n\n"
                                 "Insert:\n"
                                 "insert into Customer values (...)\n\n"
                                 "Delete:\n"
                                 "delete from Customer where DNI = x");
    });
    // ------------------------------------------------------------------------

    window.setLayout(mainLayout);
    window.resize(400, 300);
    window.show();

    return app.exec();
}
