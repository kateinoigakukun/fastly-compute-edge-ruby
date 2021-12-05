require "fastly/compute_edge"
require "json"
require "rbconfig"

body = Fastly::Body.new
resp = Fastly::Response.new

body.write JSON.pretty_generate(RbConfig::CONFIG)
resp.send_downstream body
