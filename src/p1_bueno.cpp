#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

struct Record {
  int cod;
  char name[24];
  int left;
  int right;
  int height;

  Record() : left(-1), right(-1), height(0) {}
  Record(int cod, std::string name)
    : cod(cod), left(-1), right(-1), height(0) {
    strcpy(this->name, name.c_str());
  }
};

std::ostream &operator<<(std::ostream &os, Record &record) { 
    os << record.cod << " " << record.name << " " << record.left << " " << record.right << " " << record.height << "\n"; 
    return os; 
};

//--------------------------------------------------------------

typedef int TK;
typedef int FileIndex;
class AVLFile {
   private:
  std::string filename;
  int root;

   public:
  AVLFile(std::string filename);
  void insert(Record &record);
  Record find(TK value);
  int height();
  void displayInOrder();
  ~AVLFile() {}

  // Display
  void display();

   private:
  Record find(int pos, TK value, std::fstream &file);
  void displayInOrder(std::fstream &file, int &pos);

  /*add for avl*/
  int height(Record &node);
  void insert(std::fstream &file, TK &value, int &new_pos, int actual_pos,
        int parent_pos);
  //
  void updateHeader(std::fstream &file, int new_root);
  int get_cod(std::fstream &file, int pos);
  int get_left(std::fstream &file, int pos);
  void write_left(std::fstream &file, int pos, int left);
  int get_right(std::fstream &file, int pos);
  void write_right(std::fstream &file, int pos, int right);
  int get_height(std::fstream &file, int pos);
  void write_height(std::fstream &file, int pos, int height);

  void updateHeight(std::fstream &file, int pos);
  void balance(std::fstream &file, int pos, int parent_pos);
  int balancingFactor(std::fstream &file, int pos);
  void left_rota(std::fstream &file, int pos, int parent_pos);
  void right_rota(std::fstream &file, int pos, int parent_pos);
};

AVLFile::AVLFile(std::string filename) : filename(filename) {
  std::fstream file(this->filename, std::ios::app | std::ios::binary |
                      std::ios::in | std::ios::out);

  file.seekg(0, std::ios::end);
  int bytes = file.tellg();

  if (bytes == 0) {
    std::cout << "Creating file..." << std::endl;
    this->root = -1;
    file.write((char *)&this->root, sizeof(int));
  } else {
    file.seekg(0, std::ios::beg);
    std::cout << "Reading file..." << std::endl;
    file.read((char *)&this->root, sizeof(int));
  }
  std::cout << "Init complete!" << std::endl;
  file.close();
}

void AVLFile::insert(Record &record) {
  std::fstream file(this->filename,
            std::ios::binary | std::ios::in | std::ios::out);
  // Insertar en el archivo
  file.seekp(0, std::ios::end);
  int pos = file.tellp();
  pos -= sizeof(int);	 // Restar header (root)
  pos /= sizeof(Record);

  file.write((char *)&record, sizeof(Record));
  // Enlazarlo en su posicion (left o right)
  // Actualizar la altura del nodo
  // balancear

  if (this->root == -1) {
    updateHeader(file, pos);
    return;
  }

  insert(file, record.cod, pos, this->root, -1);
  file.close();
}

void AVLFile::insert(std::fstream &file, TK &value, int &new_pos,
           int actual_pos, int parent_pos) {
  int current_cod = get_cod(file, actual_pos);

  // if (value == current_cod) return;
  if (value < current_cod) {
    if (get_left(file, actual_pos) == -1) {
      write_left(file, actual_pos, new_pos);
    } else
      insert(file, value, new_pos, get_left(file, actual_pos),
           actual_pos);
  } else if (value > current_cod) {
    if (get_right(file, actual_pos) == -1) {
      write_right(file, actual_pos, new_pos);
    } else
      insert(file, value, new_pos, get_right(file, actual_pos),
           actual_pos);
  }

  updateHeight(file, actual_pos);
  balance(file, actual_pos, parent_pos);
}

Record AVLFile::find(TK value) {
  std::fstream file(this->filename, std::ios::binary | std::ios::in);
  int pos = this->root;
  Record foundedRecord = find(pos, value, file);
  file.close();
  return foundedRecord;
}

Record AVLFile::find(int pos, TK value, std::fstream &file) {
  file.seekg((pos * sizeof(Record)) + sizeof(int), std::ios::beg);
  Record node;
  file.read((char *)&node, sizeof(Record));

  if (node.cod == value)
    return node;
  else if (value < node.cod)
    return find(node.left, value, file);
  else if (value > node.cod)
    return find(node.right, value, file);
  else
    std::cerr << "Error: value not found" << std::endl;

  return node;
}

void AVLFile::displayInOrder(std::fstream &file, int &pos) {
  // READ node
  Record node;
  file.seekg(pos * sizeof(Record) + sizeof(int), std::ios::beg);
  file.read((char *)&node, sizeof(Record));

  if (node.left != -1) displayInOrder(file, node.left);
  std::cout << node.cod << " - " << node.name << " LEFT: " << node.left
        << "  RIGHT: " << node.right << std::endl;
  if (node.right != -1) displayInOrder(file, node.right);
}

void AVLFile::displayInOrder() {
  std::fstream file(this->filename, std::ios::binary | std::ios::in);
  int pos = this->root;
  std::cout << "root: " << pos << std::endl;
  displayInOrder(file, pos);
  file.close();
}

/* add for AVL*/

int AVLFile::balancingFactor(std::fstream &file, int pos) {
  int left = get_left(file, pos);
  int right = get_right(file, pos);

  if (left != -1) left = get_height(file, left);
  if (right != -1) right = get_height(file, right);

  return left - right;
}

void AVLFile::updateHeight(std::fstream &file, int pos) {
  int left = get_left(file, pos);
  int right = get_right(file, pos);

  if (left != -1) left = get_height(file, left);
  if (right != -1) right = get_height(file, right);

  int new_height = std::max(left, right) + 1;
  write_height(file, pos, new_height);
}

void AVLFile::balance(std::fstream &file, int pos, int parent_pos) {
  int hb = balancingFactor(file, pos);
  if (hb >= 2) {	// cargado por la izquierda
    //<=-1, rotacion doble izquierda-derecha
    if (balancingFactor(file, get_left(file, pos)) <= 1)
      left_rota(file, get_left(file, pos), pos);
    right_rota(file, pos, parent_pos);
  } else if (hb <= -2) {	// cargado por la derecha
    if (balancingFactor(file, get_right(file, pos)) >=
      1)	//>=1, rotacion doble derecha-izquierda
      right_rota(file, get_right(file, pos), pos);
    left_rota(file, pos, parent_pos);
  }
}

void AVLFile::left_rota(std::fstream &file, int pos, int parent_pos) {
  int right = get_right(file, pos);  // temp
  write_right(file, pos, get_left(file, right));
  write_left(file, right, pos);
  updateHeight(file, pos);
  updateHeight(file, right);

  if (parent_pos == -1)
    updateHeader(file,
           right);  // header update cuando parent_post sea el root
  else if (get_left(file, parent_pos) == pos)
    write_left(file, parent_pos, right);
  else
    write_right(file, parent_pos, right);
}

void AVLFile::right_rota(std::fstream &file, int pos, int parent_pos) {
  int left = get_left(file, pos);
  write_left(file, pos, get_right(file, left));
  write_right(file, left, pos);
  updateHeight(file, pos);
  updateHeight(file, left);

  if (parent_pos == -1)
    updateHeader(file,
           left);	 // header update cuando parent_post sea el root
  else if (get_left(file, parent_pos) == pos)
    write_left(file, parent_pos, left);
  else
    write_right(file, parent_pos, left);
}

void AVLFile::updateHeader(std::fstream &file, int new_root) {
  file.seekp(0, std::ios::beg);
  file.write((char *)&new_root, sizeof(int));
  this->root = new_root;
}

int AVLFile::get_cod(std::fstream &file, int pos) {
  int cod;
  file.seekg(sizeof(Record) * pos + sizeof(int), std::ios::beg);
  file.read((char *)&cod, sizeof(int));
  return cod;
}

int AVLFile::get_left(std::fstream &file, int pos) {
  int left;
  file.seekg(sizeof(Record) * pos + 2 * sizeof(int) + sizeof(char[24]),
         std::ios::beg);
  file.read((char *)&left, sizeof(int));
  return left;
}

void AVLFile::write_left(std::fstream &file, int pos, int left) {
  file.seekp(sizeof(Record) * pos + 2 * sizeof(int) + sizeof(char[24]),
         std::ios::beg);
  file.write((char *)&left, sizeof(int));
  return;
}

int AVLFile::get_right(std::fstream &file, int pos) {
  int right;
  file.seekg(sizeof(Record) * pos + 3 * sizeof(int) + sizeof(char[24]),
         std::ios::beg);
  file.read((char *)&right, sizeof(int));
  return right;
}

void AVLFile::write_right(std::fstream &file, int pos, int right) {
  file.seekp(sizeof(Record) * pos + 3 * sizeof(int) + sizeof(char[24]),
         std::ios::beg);
  file.write((char *)&right, sizeof(int));
  return;
}

int AVLFile::get_height(std::fstream &file, int pos) {
  int height;
  file.seekg(sizeof(Record) * pos + 4 * sizeof(int) + sizeof(char[24]),
         std::ios::beg);
  file.read((char *)&height, sizeof(int));
  return height;
}

void AVLFile::write_height(std::fstream &file, int pos, int height) {
  file.seekp(sizeof(Record) * pos + 4 * sizeof(int) + sizeof(char[24]),
         std::ios::beg);
  file.write((char *)&height, sizeof(int));
  return;
}

void AVLFile::display() {
  std::fstream file(this->filename, std::ios::binary | std::ios::in);
  int pos;
  Record node;
  file.seekg(0, std::ios::beg);
  file.read((char *)&pos, sizeof(int));
  std::cout << "root: " << pos << std::endl;

  while (!file.eof()) {
    file.read((char *)&node, sizeof(Record));
    std::cout << node.cod << " - " << node.name << " LEFT: " << node.left
          << "  RIGHT: " << node.right << std::endl;
  }
  file.close();
}

// --------------------------------------------------

void writeFile(std::string filename) {
  AVLFile file(filename);
  Record r1{1, "MARCELO1"};
  Record r2{2, "MARCELO2"};
  Record r3{3, "MARCELO3"};
  Record r4{4, "MARCELO4"};
  Record r5{5, "MARCELO5"};
  Record r6{6, "MARCELO6"};
  Record r7{7, "MARCELO7"};
  Record r8{8, "MARCELO8"};
  Record r9{9, "MARCELO9"};
  //cout << "CHECK: " << r1.left << " "<< r1.right << " " << r1.height << endl;
  file.insert(r1);
  file.insert(r3);
  file.insert(r2);
  file.insert(r4);
  file.insert(r9);
  file.insert(r7);
  file.insert(r5);
  file.insert(r6);
  file.insert(r8);
}

void readFile(std::string filename) {
  AVLFile file(filename);
  std::cout << "--------- show all sorted data -----------\n";
  file.displayInOrder();
  std::cout << "--------- show all unsorted data -----------\n";
  file.display();
}

int main1() {
  std::cout << "START!" << std::endl;
  writeFile("MYDAT.DAT");
  readFile("MYDAT.DAT");
  std::cout << "DONE!" << std::endl;
  return 0;
}