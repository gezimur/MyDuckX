#include "duckx.hpp"

// Hack on pugixml
// We need to write xml to std string (or char *)
// So overload the write function
struct xml_string_writer: pugi::xml_writer {
    std::string result;

    virtual void write(const void* data, size_t size) {
        result.append(static_cast<const char*>(data), size);
    }
};

void duckx::Run::set_parent(pugi::xml_node node) {
    this->parent = node;
    this->current = this->parent.child("w:r");
}

const pugi::char_t *duckx::Run::text() {
    return this->current.child("w:t").text().get();
}

const duckx::Run& duckx::Run::next() {
    this->current = this->current.next_sibling();
    return *this;
}

bool duckx::Run::has_next() {
    return this->current != 0;
}

duckx::Paragraph::Paragraph() {}

void duckx::Paragraph::set_parent(pugi::xml_node node) {
    this->parent = node;
    this->current = this->parent.child("w:p");
    
    this->run.set_parent(
        this->current
    );
}

const duckx::Paragraph &duckx::Paragraph::next() {
    this->current = this->current.next_sibling();
    this->run.set_parent(this->current);
    return *this;
}

bool duckx::Paragraph::has_next() {
    return this->current != 0;
}

duckx::Run &duckx::Paragraph::runs() {
    return this->run;
}


duckx::Document::Document() {
    // TODO: this function must be removed!
    this->directory = "";
}

duckx::Document::Document(std::string directory) {
    this->directory = directory;
}

void duckx::Document::file(std::string directory) {
    this->directory = directory;
}

void duckx::Document::open() {
    void *buf = NULL;
    size_t bufsize;

    // Open file and load "xml" content to the document variable
    zip_t *zip = zip_open(this->directory.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
    
    zip_entry_open(zip, "word/document.xml");
    zip_entry_read(zip, &buf, &bufsize);
    
    zip_entry_close(zip);
    zip_close(zip);

    this->document.load_string(
        (char *) buf
    );

    free(buf);

    this->paragraph.set_parent(
        document.child("w:document").child("w:body")
    );
}

void duckx::Document::save() {
    // Open file and replace "xml" content
    zip_t *zip = zip_open(this->directory.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'a');
    
    zip_entry_open(zip, "word/document.xml");
    

    xml_string_writer writer;
    this->document.print(writer);

    const char *buf = writer.result.c_str();
    zip_entry_write(zip, buf, strlen(buf));
    
    zip_entry_close(zip);
    zip_close(zip);
}

duckx::Paragraph &duckx::Document::paragraphs() {
    return this->paragraph;
}