# 🔐 LSB-Based Image Steganography Using C

### An ordinary image, an invisible secret!

## 📌 About the Project

Developed an LSB-Based Image Steganography application using C to hide secret files inside BMP images and retrieve them through encoding and decoding operations.

The project uses the **Least Significant Bit (LSB) technique** to embed secret data into image pixel bytes while maintaining almost the same visual appearance.

## ✨ Key Features

* Hide secret files inside BMP images.
* Extract hidden files through decoding.
* Encode magic string, file extension, and file size.
* Check image capacity before encoding.
* Preserve the original BMP header.
* Support secret file encoding and decoding.

## ⚙️ Encoding & Decoding Workflow

**Encoding:**

1. Validate input files.
2. Check image capacity.
3. Copy BMP header.
4. Encode magic string.
5. Encode secret file extension size and extension.
6. Encode secret file size.
7. Encode secret file data.

**Decoding:**

1. Read encoded BMP image.
2. Verify magic string.
3. Extract extension size and extension.
4. Retrieve secret file size.
5. Extract and reconstruct the original file.

## 💻 C Concepts Applied

* Pointers
* Structures
* Macros
* File Handling
* Bitwise Operations
* Arrays and Strings
* Functions and Modular Programming
* Command-Line Arguments

## 🛠️ Technologies Used

* C Programming
* Linux
* GCC Compiler
* BMP Image Format

## 🚀 Compilation & Execution

**Compile:**

```bash
gcc *.c -o stego
```

**Encoding:**

```bash
./stego -e input.bmp secret.txt
```

**Decoding:**

```bash
./stego -d stego.bmp
```

## 📚 Key Learning

Gained hands-on experience in Advanced C Programming, file handling, bitwise manipulation, pointers, structures, and understanding how data is stored and retrieved at the bit level.

## 👨‍💻 Author

B. Dayakar Reddy

B.Tech | Aspiring Embedded Systems Engineer

⭐ Exploring the power of bits, one image at a time!
