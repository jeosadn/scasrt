module asrt;
   int fd;
   bit clk = 0;
   bit [31:0] addr = 0;

   task automatic addr_event(
      input bit [31:0] set_val
   );
      $fdisplay(fd, $sformatf("addr=0x%08x", set_val));
      addr <= set_val;
      clk <= 0;
      #1;

      clk <= 1;
      #1;
   endtask : addr_event

   initial begin
      $display($sformatf("========================================"));
      $display($sformatf("========================================"));
      $display($sformatf("Assertion testing"));

      `ifdef CONCAT_PASS
      fd = $fopen("../CONCAT_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: CONCAT_PASS"));
      addr_event(1);
      addr_event(2);
      `endif // CONCAT_PASS

      `ifdef CONCAT_FAIL
      fd = $fopen("../CONCAT_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: CONCAT_FAIL"));
      addr_event(1);
      addr_event(3);
      `endif // CONCAT_FAIL

      `ifdef ROSE_PASS
      fd = $fopen("../ROSE_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: ROSE_PASS"));
      addr_event(0);
      addr_event(1);
      `endif // ROSE_PASS

      `ifdef ROSE_FAIL
      fd = $fopen("../ROSE_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: ROSE_FAIL"));
      addr_event(0);
      addr_event(0);
      `endif // ROSE_FAIL

      `ifdef FELL_PASS
      fd = $fopen("../FELL_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: FELL_PASS"));
      addr_event(1);
      addr_event(0);
      `endif // FELL_PASS

      `ifdef FELL_FAIL
      fd = $fopen("../FELL_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: FELL_FAIL"));
      addr_event(1);
      addr_event(1);
      `endif // FELL_FAIL

      `ifdef PAST_PASS
      fd = $fopen("../PAST_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: PAST_PASS"));
      addr_event(1);
      addr_event(0);
      `endif // PAST_PASS

      `ifdef PAST_FAIL
      fd = $fopen("../PAST_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: PAST_FAIL"));
      addr_event(1);
      addr_event(0);
      `endif // PAST_FAIL

      `ifdef STABLE_PASS
      fd = $fopen("../STABLE_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: STABLE_PASS"));
      addr_event(1);
      addr_event(1);
      `endif // STABLE_PASS

      `ifdef STABLE_FAIL
      fd = $fopen("../STABLE_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: STABLE_FAIL"));
      addr_event(1);
      addr_event(0);
      `endif // STABLE_FAIL

      `ifdef INTERSECT_PASS
      fd = $fopen("../INTERSECT_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: INTERSECT_PASS"));
      addr_event(1);
      addr_event(3);
      addr_event(0);
      `endif // INTERSECT_PASS

      `ifdef INTERSECT_FAIL
      fd = $fopen("../INTERSECT_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: INTERSECT_FAIL"));
      addr_event(1);
      addr_event(3);
      addr_event(1);
      `endif // INTERSECT_FAIL

      `ifdef THROUGHOUT_PASS
      fd = $fopen("../THROUGHOUT_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: THROUGHOUT_PASS"));
      addr_event(2);
      addr_event(3);
      `endif // THROUGHOUT_PASS

      `ifdef THROUGHOUT_FAIL
      fd = $fopen("../THROUGHOUT_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: THROUGHOUT_FAIL"));
      addr_event(2);
      addr_event(6);
      `endif // THROUGHOUT_FAIL

      `ifdef WITHIN_PASS
      fd = $fopen("../WITHIN_PASS_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: WITHIN_PASS"));
      addr_event('b0001);
      addr_event('b0011);
      addr_event('b0101);
      addr_event('b1010);
      `endif // WITHIN_PASS

      `ifdef WITHIN_FAIL
      fd = $fopen("../WITHIN_FAIL_sv.result", "w");
      // Stimulus
      $fdisplay(fd, $sformatf("Test: WITHIN_FAIL"));
      addr_event('b0001);
      addr_event('b0001);
      addr_event('b0101);
      addr_event('b1010);
      `endif // WITHIN_FAIL


      $display($sformatf("========================================"));
      $display($sformatf("========================================"));
   end

   `ifdef CONCAT_PASS
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> (addr == 2);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //CONCAT_PASS

   `ifdef CONCAT_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> (addr == 2);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //CONCAT_FAIL

   `ifdef ROSE_PASS
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 0) |=> $rose(addr[0:0]);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //ROSE_PASS

   `ifdef ROSE_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 0) |=> $rose(addr[0:0]);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //ROSE_FAIL

   `ifdef FELL_PASS
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> $fell(addr[0:0]);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //FELL_PASS

   `ifdef FELL_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> $fell(addr[0:0]);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //FELL_FAIL

   `ifdef PAST_PASS
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> ($past(addr, 1) == 1);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //PAST_PASS

   `ifdef PAST_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> ($past(addr, 1) == 0);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //PAST_FAIL

   `ifdef STABLE_PASS
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> $stable(addr[0:0]);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //STABLE_PASS

   `ifdef STABLE_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk) (addr == 1) |=> $stable(addr[0:0]);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //STABLE_FAIL

   `ifdef INTERSECT_PASS
   // Assertion
   property asrt_1;
      @(posedge clk)
      addr[1:1] == 1 |-> ((addr[1:1] == 1 ##1 addr[1:1] == 0) intersect (addr[0:0] == 1 ##1 addr[0:0] == 0));
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //INTERSECT_PASS

   `ifdef INTERSECT_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk)
      addr[1:1] == 1 |-> ((addr[1:1] == 1 ##1 addr[1:1] == 0) intersect (addr[0:0] == 1 ##1 addr[0:0] == 0));
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //INTERSECT_FAIL

   `ifdef THROUGHOUT_PASS
   // Assertion
   property asrt_1;
      @(posedge clk)
      addr[1:1] == 1 |=> (addr[1:1] == 1 throughout (addr[0:0] == 1));
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //THROUGHOUT_PASS

   `ifdef THROUGHOUT_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk)
      addr[1:1] == 1 |=> (addr[1:1] == 1 throughout (addr[0:0] == 1));
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //THROUGHOUT_FAIL

   `ifdef WITHIN_PASS
   // Assertion
   property asrt_1;
      @(posedge clk)
      addr == 1 |=> (addr[1:1] == 1 ##1 addr[1:1] == 0) within (addr[0:0] == 1 ##2 addr[0:0] == 0);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //WITHIN_PASS

   `ifdef WITHIN_FAIL
   // Assertion
   property asrt_1;
      @(posedge clk)
      addr == 1 |=> (addr[1:1] == 1 ##1 addr[1:1] == 0) within (addr[0:0] == 1 ##2 addr[0:0] == 0);
   endproperty : asrt_1;
   assert property(asrt_1) $fdisplay(fd, $sformatf("PASS"));
   else  $fdisplay(fd, $sformatf("FAIL"));
   `endif //WITHIN_FAIL
endmodule : asrt
