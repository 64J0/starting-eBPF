# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure("2") do |config|
  # config.vm.box = "ubuntu/focal64"     # Ubuntu 20.04 Focal Fossa (non CO-RE)
  config.vm.box = "ubuntu/jammy64"       # Ubuntu 22.04 Jammy Jellyfish (CO-RE)

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # NOTE: This will enable public access to the opened port
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine and only allow access
  # via 127.0.0.1 to disable public access
  # config.vm.network "forwarded_port", guest: 80, host: 8080, host_ip: "127.0.0.1"

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # subnet: 192.168.56.0/21
  config.vm.network "private_network", ip: "192.168.56.10"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  config.ssh.extra_args = ["-t", "cd /vagrant; bash --login"]

  config.vm.provider "virtualbox" do |vb|
    vb.gui = false
    vb.cpus = "2"
    vb.memory = "2048"
    vb.name = "starting-ebpf"
  end

  # Based on:
  # https://github.com/lizrice/learning-ebpf/blob/main/learning-ebpf.yaml
  config.vm.provision "shell", inline: <<-SHELL
    VAGRANT_HOME="/home/vagrant"
    DEBIAN_FRONTEND="noninteractive"

    apt-get update
    apt-get install -y apt-transport-https ca-certificates curl jq
    apt-get install -y clang llvm
    apt-get install -y libelf-dev libpcap-dev libbfd-dev binutils-dev
    apt-get install -y build-essential make
    apt-get install -y linux-tools-common linux-tools-5.15.0-41-generic
    apt-get install -y bpfcc-tools linux-headers-$(uname -r)
    apt-get install -y python3-pip
  SHELL
end
