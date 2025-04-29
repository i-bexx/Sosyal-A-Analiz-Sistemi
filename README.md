# Sosyal Ağ Analizi — C Dili ile Kırmızı-Siyah Ağaç ve Grafik İşlemleri

## Proje Açıklaması

Bu proje, bir sosyal ağ grafiğini **ağaç tabanlı veri yapıları** ile modellemektedir.  
Kullanıcılar arasındaki ilişkiler **graf** yapısında tutulurken, **analiz işlemleri** için özellikle **kırmızı-siyah ağaç (Red-Black Tree)** ve **DFS tabanlı arama** yapıları kullanılmıştır.

## Kullanılan Veri Yapıları

- **Graf:**  
  Kullanıcıların arkadaşlık ilişkileri bu yapıda tutulur.

- **Red-Black Tree (Kırmızı-Siyah Ağaç):**  
  Kullanıcı kimliklerine göre **dengeli** bir şekilde arama ve ekleme yapılmasını sağlar.



##  Veri Formatı (veriseti.txt)

```txt
# Kullanıcılar
USER 101
USER 102
USER 103
USER 104
USER 105

# Arkadaşlık ilişkileri
FRIEND 101 102
FRIEND 101 103
FRIEND 102 104
FRIEND 103 104
FRIEND 104 105

