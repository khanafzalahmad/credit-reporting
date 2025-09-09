terraform {
  required_version = ">= 1.5.0"
  required_providers {
    kubernetes = {
      source  = "hashicorp/kubernetes"
      version = "~> 2.31" # recent provider
    }
  }
}

provider "kubernetes" {
  config_path = var.kubeconfig_path
}
resource "kubernetes_namespace" "ns" {
  metadata { name = var.namespace }
}

# Optional: allow pulling private GHCR images
resource "kubernetes_secret" "ghcr_pull" {
  count = var.create_image_pull_secret ? 1 : 0

  metadata {
    name      = "ghcr-pull-secret"
    namespace = kubernetes_namespace.ns.metadata[0].name
  }

  type = "kubernetes.io/dockerconfigjson"

  data = {
    ".dockerconfigjson" = jsonencode({
      auths = {
        "ghcr.io" = {
          username = var.ghcr_username
          password = var.ghcr_token
          auth     = base64encode("${var.ghcr_username}:${var.ghcr_token}")
        }
      }
    })
  }
}

resource "kubernetes_deployment" "app" {
  metadata {
    name      = var.app_name
    namespace = kubernetes_namespace.ns.metadata[0].name
    labels    = { app = var.app_name }
  }

  spec {
    replicas = var.replicas

    selector {
      match_labels = { app = var.app_name }
    }

    template {
      metadata { labels = { app = var.app_name } }

      spec {
        dynamic "image_pull_secrets" {
          for_each = var.create_image_pull_secret ? [1] : []
          content {
            name = kubernetes_secret.ghcr_pull[0].metadata[0].name
          }
        }

        container {
          name  = var.app_name
          image = var.image

          port { 
                 name = "http"
                 container_port = 8080
               }

          readiness_probe {
            http_get { 
                       path = "/healthz" 
                       port = "http" 
                     }
            initial_delay_seconds = 3
            period_seconds        = 10
          }

          liveness_probe {
            http_get { 
                       path = "/healthz"
                       port = "http" 
                     }
            initial_delay_seconds = 10
            period_seconds        = 20
          }

          resources {
            requests = { cpu = "100m", memory = "128Mi" }
            limits   = { cpu = "500m", memory = "512Mi" }
          }
        }
      }
    }
  }
}

resource "kubernetes_service" "svc" {
  metadata {
    name      = var.app_name
    namespace = kubernetes_namespace.ns.metadata[0].name
    labels    = { app = var.app_name }
  }

  spec {
    selector = { app = var.app_name }
    type     = "NodePort"

    port {
      name        = "http"
      port        = 80
      target_port = 8080
      node_port   = var.node_port
    }
  }
}
