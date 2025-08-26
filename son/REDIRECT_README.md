# Minishell Redirect Integration Guide

Bu dosya, Redirect klasöründeki redirect işlevselliğini mevcut minishell'inize nasıl entegre edeceğinizi açıklar.

## 🚀 Yeni Özellikler

### 1. Gelişmiş Redirect Yönetimi
- **Input/Output Redirects**: `<`, `>`, `>>` operatörleri
- **Heredoc**: `<<` operatörü ile çok satırlı input
- **Variable Expansion**: Heredoc içinde `$VAR` desteği
- **Signal Handling**: Ctrl+C ve Ctrl+D desteği

### 2. Modern Yapılar
- `t_req`: Request yapısı (command ve exit status bilgileri)
- `t_pipeline_data`: Pipeline veri yapısı
- Gelişmiş error handling ve memory management

## 📁 Yeni Dosyalar

```
src/redirection/
├── redirect_handler.c          # Ana redirect işleyici
├── heredoc_handler.c           # Heredoc işleyici
├── heredoc_handler_utils.c     # Heredoc yardımcı fonksiyonları
└── redirect_integration.c      # Entegrasyon fonksiyonları
```

## 🔧 Kullanım

### Temel Redirect Setup

```c
// Command için redirect'leri işle
if (process_command_redirections(cmd, shell) != 0)
{
    // Error handling
    return (1);
}

// Redirect'leri uygula
if (apply_command_redirections(cmd) != 0)
{
    // Error handling
    return (1);
}

// Command'i çalıştır
execute_command(cmd);

// Redirect'leri geri al
restore_command_redirections(cmd);
```

### Heredoc Kullanımı

```c
// Heredoc işle
int heredoc_fd = handle_heredoc_enhanced("EOF", shell);
if (heredoc_fd == -1)
{
    // Error handling
    return (1);
}

// Heredoc fd'sini kullan
dup2(heredoc_fd, STDIN_FILENO);
close(heredoc_fd);
```

### Redirect Validation

```c
// Redirect'leri doğrula
if (validate_redirections_enhanced(cmd) != 0)
{
    // Warning veya error handling
}
```

## 🏗️ Entegrasyon Adımları

### 1. Header Güncellemeleri
`minishell.h` dosyasına yeni yapılar ve fonksiyonlar eklendi:
- `t_req` ve `t_pipeline_data` yapıları
- Redirect fonksiyonları
- Integration fonksiyonları

### 2. Command Yapısı Güncellemeleri
`t_command` yapısına yeni alanlar eklendi:
- `char **envp`: Environment variables
- `int infile`: Input file descriptor
- `int outfile`: Output file descriptor

### 3. Makefile Güncellemeleri
Yeni redirect dosyaları Makefile'a eklendi.

## 🔄 Mevcut Kod ile Uyumluluk

### Eski Fonksiyonlar
Eski redirect fonksiyonları hala çalışır durumda:
- `setup_redirections()`
- `restore_redirections()`
- `validate_redirections()`

### Yeni Fonksiyonlar
Yeni fonksiyonlar eski fonksiyonların yerini alabilir:
- `setup_redirections_enhanced()` → `setup_redirections()`
- `process_command_redirections()` → Tüm redirect işlemlerini tek seferde yapar

## 🧪 Test Etme

### Basit Redirect Testi
```bash
echo "test" > output.txt
cat < input.txt
echo "append" >> output.txt
```

### Heredoc Testi
```bash
cat << EOF
line 1
line 2
$USER
EOF
```

### Variable Expansion Testi
```bash
cat << EOF
Current user: $USER
Current path: $PWD
EOF
```

## ⚠️ Önemli Notlar

1. **Environment Variables**: Command'larda `envp` alanının doldurulması gerekir
2. **File Descriptors**: Redirect'lerden sonra file descriptor'ların kapatılması gerekir
3. **Signal Handling**: Heredoc sırasında Ctrl+C ve Ctrl+D düzgün işlenir
4. **Memory Management**: Tüm memory allocation'lar otomatik olarak temizlenir

## 🐛 Hata Ayıklama

### Yaygın Hatalar
- `envp` NULL pointer hatası
- File descriptor leak
- Memory leak

### Debug İpuçları
- Valgrind ile memory leak kontrolü
- File descriptor sayısı kontrolü (`ls /proc/$$/fd`)
- Signal handling testleri

## 📚 Ek Kaynaklar

- `man 2 open` - File operations
- `man 2 dup2` - File descriptor duplication
- `man 2 pipe` - Pipe operations
- `man 2 fork` - Process creation

## 🤝 Katkıda Bulunma

Bu redirect sistemi sürekli geliştirilmektedir. Öneriler ve iyileştirmeler için:
1. Kod kalitesi standartlarına uyun
2. Memory leak olmamasına dikkat edin
3. Error handling'i düzgün yapın
4. Test coverage'ı artırın
