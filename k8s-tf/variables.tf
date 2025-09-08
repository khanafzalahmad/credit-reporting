variable "namespace" {
  description = "Kubernetes namespace"
  type        = string
  default     = "credit-reporting"
}

variable "app_name" {
  description = "App label/name"
  type        = string
  default     = "credit-report-web"
}

variable "image" {
  description = "Container image (e.g., ghcr.io/owner/repo:tag)"
  type        = string
}

variable "replicas" {
  description = "Deployment replicas"
  type        = number
  default     = 3
}

variable "node_port" {
  description = "NodePort to expose (optional fixed)"
  type        = number
  default     = 30080
}

variable "kubeconfig_path" {
  description = "Path to kubeconfig on runner"
  type        = string
  default     = "~/.kube/config"
}

variable "create_image_pull_secret" {
  description = "Create imagePullSecret for private GHCR"
  type        = bool
  default     = false
}

variable "ghcr_username" {
  description = "GHCR username (only if create_image_pull_secret=true)"
  type        = string
  default     = ""
}

variable "ghcr_token" {
  description = "GHCR token/password (only if create_image_pull_secret=true)"
  type        = string
  default     = ""
  sensitive   = true
}
