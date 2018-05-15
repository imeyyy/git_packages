class ClientCommunicator {
	constructor(params) {
		console.log("Started Client Communicator")
		this.rotate_degrees = 5
		this.initalized = false
		this.stopped = true
		this.forward = 0
		this.back = 0
		this.left = 0
		this.right = 0
		this.rotate_left = 0
		this.rotate_right = 0
	}

	do(params) {
		let data = params.data
		let callback = params.callback
		$.ajax({
			type: 'POST',
			url: '/',
			data: data,
			dataType: 'json',
			success: function(data) {
				var result = JSON.parse(data)
				console.log(result);
				client_communicator.callback(result)
			}
		});
	}

	callback(result) {
		$('#log_box').html(result.command)
		
		if (result.command === 'turn_on') {
			this.initalized = true
		}

		if (result.command === 'turn_off') {
			this.initalized = false
		}

		if (result.command === 'stop') {
			this.stopped = true
		}

		if (result.command === 'forward') {
			this.forward += 1
		}

		if (result.command === 'back') {
			this.back+= 1
		}

		if (result.command === 'left') {
			this.left += 1
		}

		if (result.command === 'right') {
			this.right += 1
		}

		if (result.command === 'rotate_left') {
			this.rotate_left += client_communicator.rotate_degrees
		}

		if (result.command === 'rotate_right') {
			this.rotate_right += client_communicator.rotate_degrees
		}
	}
}

var client_communicator = new ClientCommunicator

$(document).ready(function () {

	$('#turn_on').click(function () {
		client_communicator.do({
			data: {"command":"turn_on", "args":""}
		})
	})

    $('#turn_off').click(function () {
		client_communicator.do({
			data: {"command":"turn_off", "args":""}
		})
	})

	$('#stop').click(function () {
		client_communicator.do({
			data: {"command":"stop", "args":"--once"}
		})
	})

	$('#forward').click(function () {
		client_communicator.do({
			data: {"command":"forward", "args":"--once"}
		})
	})

	$('#back').click(function () {
		client_communicator.do({
			data: {"command":"back", "args":"--once"}
		})
	})

	$('#left').click(function () {
		client_communicator.do({
			data: {"command":"left", "args":"--once"}
		})
	})

	$('#right').click(function () {
		client_communicator.do({
			data: {"command":"right", "args":"--once"}
		})
	})

	$('#rotate_left').click(function () {
		client_communicator.do({
			data: {"command":"rotate_left", "args":client_communicator.rotate_degrees}
		})
	})

	$('#rotate_right').click(function () {
		client_communicator.do({
			data: {"command":"rotate_right", "args":client_communicator.rotate_degrees}
		})
	})

})