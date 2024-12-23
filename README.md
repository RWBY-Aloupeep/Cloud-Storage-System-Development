# Cloud-Storage-System-Development

## Introduction
A cloud storage system with a client-server architecture using the Qt framework and C++.

We developed a **cloud storage system** using the **Qt framework** and **C++**. Our system employed a **client-server architecture** utilizing **Sqlite3** for user data storage. The system successfully achieved **basic functions** common in cloud storage services. Users can register and log in securely, manage contacts, and engage in private and group chats. The system also allows users to upload, download, rename, delete, and share files. Users can also create, delete, rename, and navigate folders.

The files uploaded by users to the server are encrypted and cannot be directly accessed from the server. This implementation uses **AES encryption and decryption** functions provided by **OpenSSL** to ensure the security of the files. 

For thread management, we chose **multithreading** instead of a **thread pool** because it offers greater flexibility in thread management. Each thread can be independently created and controlled, making it ideal for tasks with varying execution times or resource needs. For simple, occasional tasks, creating threads on demand avoids the complexity and resource overhead of managing a thread pool. This approach also allows for immediate thread destruction, preventing idle threads that may arise in a thread pool. 
    
Additionally, to manage system resources, we will implement a mechanism to **clean up inactive connections** using a list instead of a max-heap, as mentioned in the proposal.
On top of that, we use the **MD5 algorithm** for the integrity check instead of **SHA-256** as mentioned in the proposal.

## Installation
1. **Include QT Modules:**
   add network in TcpClient.pro
   add network and sql in TcpServer.pro
2. **Install zip and unzip**
3. **Install OpenSSL:**
   1. Download OpenSSL
   2. Include the path in TcpClient.pro
      ```cpp
      INCLUDEPATH += $$quote(C:/Program Files/OpenSSL-Win64/include)
      LIBS += $$quote(C:/Program Files/OpenSSL-Win64/lib/libssl.lib) \
              $$quote(C:/Program Files/OpenSSL-Win64/lib/libcrypto.lib)
      ```
4. **Modify Address:**
   Change the address in opedb.cpp to the current address of cloud.db
