# frozen_string_literal: true

require_relative "compute_edge/version"
require "fastly/compute_edge_core.so"

module Fastly
  module ComputeEdge
    class Error < StandardError; end
  end

  class Body
    attr_reader :handle
    def initialize
      @handle = Body.fastly_http_body_new
    end

    def write(data, body_write_end = 0)
      Body.fastly_http_body_write(@handle, data, body_write_end)
    end
  end

  class Response
    def initialize
      @handle = Response.fastly_http_resp_new
    end

    def send_downstream(body, streaming = 0)
      Fastly::Response::fastly_http_resp_send_downstream(@handle, body.handle, streaming)
    end
  end
end
