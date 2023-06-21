# -*- mode: ruby -*-
# vi: set ft=ruby :

# Source:
# https://aquasecurity.github.io/tracee/v0.9/tutorials/setup-development-machine-with-vagrant/
# https://github.com/aquasecurity/tracee/blob/main/Vagrantfile

Vagrant.configure("2") do |config|
  # config.vm.box = "ubuntu/focal64"     # Ubuntu 20.04 Focal Fossa (non CO-RE)
  # config.vm.box = "ubuntu/hirsute64"   # Ubuntu 21.04 Hirsute Hippo (CO-RE)
  # config.vm.box = "ubuntu/impish64"    # Ubuntu 21.10 Impish Indri (CO-RE)
  config.vm.box = "ubuntu/jammy64"       # Ubuntu 22.04 Jammy Jellyfish (CO-RE)

  config.ssh.extra_args = ["-t", "cd /vagrant; bash --login"]

  # Forward MkDocs dev server to preview documentation on the host at http://localhost:8000/tracee
  config.vm.network :forwarded_port, guest: 8000, host: 8000

  # Forward MicroK8s dashboard to access it on the host at https://localhost:10443
  #
  # To access the Kubernetes dashboard from the host run the following command:
  #     kubectl port-forward --address 0.0.0.0 -n kube-system service/kubernetes-dashboard 10443:443
  #
  # To sign in use the token retrieved with
  #     token=$(microk8s kubectl -n kube-system get secret | grep default-token | cut -d " " -f1)
  #     kubectl -n kube-system describe secret $token
  #
  # TIP For Google Chrome you may allow insecure TLS connections at chrome://flags/#allow-insecure-localhost
  config.vm.network :forwarded_port, guest: 10443, host: 10443

  config.vm.provider "virtualbox" do |vb|
    vb.gui = false
    vb.cpus = 2
    vb.memory = "2048"
    vb.name = "starting-ebpf"
  end

  config.vm.provision "shell", privileged: true, inline: <<-SHELL
    VAGRANT_HOME="/home/vagrant"
    GO_VERSION="1.19.5"
    OPA_VERSION="v0.48.0"

    # silence 'dpkg-preconfigure: unable to re-open stdin: No such file or directory'
    export DEBIAN_FRONTEND=noninteractive

    apt-get update

    #
    # build environment for tracee
    #

    apt-get install --yes bsdutils
    apt-get install --yes build-essential
    apt-get install --yes pkgconf

    apt-get install --yes llvm-12 clang-12
    apt-get install --yes clang-format-12
    for tool in "clang" "llc" "llvm-strip"
    do
      path=$(which $tool-12)
      ln -s "$path" "${path%-*}"
    done

    apt-get install --yes zlib1g-dev libelf-dev
    apt-get install --yes protobuf-compiler
    apt-get install --yes linux-tools-"$(uname -r)"

    # golang
    wget --quiet https://golang.org/dl/go$GO_VERSION.linux-amd64.tar.gz
    tar -C /usr/local -xzf go$GO_VERSION.linux-amd64.tar.gz
    GOBIN_PATH=/usr/local/go/bin
    echo "export PATH=$PATH:$GOBIN_PATH" >> $VAGRANT_HOME/.profile
    # integration tests run as root, so go needs to be in root's path as well
    echo "export PATH=$PATH:$GOBIN_PATH" >> $HOME/.bashrc
    # sudo needs to be able to find go as well
    echo "Defaults secure_path=\"$PATH:$GOBIN_PATH\"" >> /etc/sudoers.d/vagrant

    #
    # microk8s
    #

    snap install microk8s --classic
    microk8s status --wait-ready
    usermod -a -G microk8s vagrant
    microk8s enable hostpath-storage dns dashboard

    mkdir -p $VAGRANT_HOME/.kube/
    microk8s kubectl config view --raw > $VAGRANT_HOME/.kube/config
    chmod 600 $VAGRANT_HOME/.kube/config
    chown vagrant:vagrant $VAGRANT_HOME/.kube/config

    #
    # kubectl
    #

    apt-get install --yes apt-transport-https ca-certificates curl
    curl -fsSL https://packages.cloud.google.com/apt/doc/apt-key.gpg | gpg --dearmor -o /etc/apt/keyrings/kubernetes-archive-keyring.gpg
    echo "deb [signed-by=/etc/apt/keyrings/kubernetes-archive-keyring.gpg] https://apt.kubernetes.io/ kubernetes-xenial main" | tee /etc/apt/sources.list.d/kubernetes.list
    apt-get update
    apt-get install --yes kubectl
    echo 'source <(kubectl completion bash)' >> $VAGRANT_HOME/.profile

    #
    # helm
    #

    snap install helm --classic
    echo 'source <(helm completion bash)' >> $VAGRANT_HOME/.profile

    #
    # docker
    #
    
    apt-get install --yes docker.io
    usermod -aG docker vagrant

    #
    # opa
    #

    curl -L -o /usr/bin/opa https://github.com/open-policy-agent/opa/releases/download/$OPA_VERSION/opa_linux_amd64
    chmod 755 /usr/bin/opa

    #
    # iovisor/bcc
    # https://github.com/iovisor/bcc/blob/master/INSTALL.md#ubuntu---binary
    #

    apt-get install bpfcc-tools linux-headers-$(uname -r)
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4052245BD4284CDD
    echo "deb https://repo.iovisor.org/apt/$(lsb_release -cs) $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/iovisor.list
    apt-get update
    apt-get install bcc-tools libbcc-examples linux-headers-$(uname -r)
  SHELL
end
