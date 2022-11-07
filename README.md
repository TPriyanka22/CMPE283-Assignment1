# CMPE283-Assignment1

Please find the steps to recreate the development steps:


●	Create a free tier account in the Google Cloud Platform to make use of the services.

●	Create a VM through UI or fill in the information like Name, InstanceId, Zone, machine type, and CPU Platform and click on the Equivalent command line button (present at the bottom of the page), cloud shell will open with the information filled similar to the screenshot below.
Below are the details that were chosen for this assignment:
Name: cmpe283-instance
Zone: us-central1-b
Machine Type: n2-standard-8
CPU Platform: Intel Cascade Lake


●	Add extra capabilities as per the requirement, here add enable nested virtualization before creating the instance.

<img width="470" alt="image" src="https://user-images.githubusercontent.com/111544172/200439363-2443d079-c109-4c1d-97fc-c0c6b62d474d.png">

●	Once the instance is created it shows the details of the instance as shown below in the cloud shell:
<img width="470" alt="image" src="https://user-images.githubusercontent.com/111544172/200439560-a4519abf-726a-4a17-96b9-d7a0a3c97b4d.png">

●	To connect to VM through SSH, click on the SSH in the instance just created:
<img width="470" alt="image" src="https://user-images.githubusercontent.com/111544172/200439608-d3c28db9-9c16-4411-8bdf-039d2f4c5c8c.png">

●	To connect to VM through SSH, click on the SSH in the instance just created:
<img width="470" alt="image" src="https://user-images.githubusercontent.com/111544172/200439657-bdbf6f22-f0df-4c0e-8509-c970c75d20a9.png">

●	Create a new directory - cmpe283A1 and clone the kernel module and Make File from this repo.

●	Go to the root folder using ‘sudo bash’ command and install gcc and make
Run the command - apt install gcc make
<img width="468" alt="image" src="https://user-images.githubusercontent.com/111544172/200439748-d7141863-1dab-4bfc-bf73-7978acf42308.png">

●	And install linux headers using the below command:
Run the command - sudo apt install linux-headers-$(uname-r)
Uname -r → 5.10.0-19-cloud-amd64 (It will give the version of the kernel that we are currently running)
<img width="468" alt="image" src="https://user-images.githubusercontent.com/111544172/200439795-69864f3f-985c-41a6-99fe-ae26abb12e33.png">


●	Now run the Make command it would appear as follows and command - sudo insmod  ./cmpe283A1.ko will help to insert the kernel module/object
and to remove the module run 'sudo rmmod cmpe283A1'

<img width="470" alt="image" src="https://user-images.githubusercontent.com/111544172/200439833-993bda4a-cfc4-44cf-8707-bc959105496e.png">

<img width="470" alt="image" src="https://user-images.githubusercontent.com/111544172/200439841-1cb5b676-8d8f-461c-b87f-fd727e9ca696.png">

•	To see the output run the command ‘sudo dmesg’: 

<img width="468" alt="image" src="https://user-images.githubusercontent.com/111544172/200439898-7508d99f-d2fe-48d3-8f1f-691720d069f7.png">

<img width="468" alt="image" src="https://user-images.githubusercontent.com/111544172/200439910-dbf107db-b4b9-4ae1-aee6-30b56241abf6.png">



